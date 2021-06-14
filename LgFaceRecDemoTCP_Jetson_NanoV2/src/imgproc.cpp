
#include <iostream>
#include <string>
#include <chrono>
#include <NvInfer.h>
#include <NvInferPlugin.h>
#include <l2norm_helper.h>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudawarping.hpp>
#include "faceNet.h"
#include "videoStreamer.h"
#include "network.h"
#include "mtcnn.h"
#include <glib.h>
#include "common.h"

#include "NetworkTCP.h"
#include "TcpSendRecvJpeg.h"
#include <termios.h>
#include "mymsg.h"
#include "imgproc.h"
#include "mydb.h"



struct facenet_data {
    FaceNetClassifier *faceNet;
    mtcnn *mtCNN;
    int maxFacesPerScene;
    int nbFrames;
    std::vector<struct Bbox> outputBbox;
    VideoStreamer *fileStreamer;
    VideoStreamer *cameraStreamer;
};


CImgProc::CImgProc(GAsyncQueue *q)
{
    main_queue = q;
    facenet_data *pfd=new facenet_data;
    pfd->fileStreamer=nullptr;
    pfd->cameraStreamer=nullptr;
    pfacenet_data=pfd;
    run_mode=IMGPROC_MODE_RUN;
    thread_run = false;
    camera_dev = "";
    imgproc_queue = g_async_queue_new();
    init_facenet();
}

CImgProc::CImgProc(GAsyncQueue *q,string dev)
{
    main_queue = q;
    facenet_data *pfd=new facenet_data;
    pfd->fileStreamer=nullptr;
    pfd->cameraStreamer=nullptr;
    pfacenet_data=pfd;
    run_mode=IMGPROC_MODE_RUN;
    thread_run = false;
    camera_dev = dev;
    imgproc_queue = g_async_queue_new();
    init_facenet();
}

CImgProc::~CImgProc()
{
    g_async_queue_unref(imgproc_queue);
    deinit_facenet();
    if (pfacenet_data) delete (facenet_data*)pfacenet_data;
}

gboolean
CImgProc::start(eImgProc_Runmode mode/*=IMGPROC_MODE_RUN*/)
{
    if (thread_run) return false;

    facenet_data *pfd=(facenet_data*)pfacenet_data;

    run_mode=mode;
    thread_run=true;
    while(g_async_queue_length (imgproc_queue)>0) {
        ImgProcMsg *pmsg = (ImgProcMsg *)g_async_queue_timeout_pop(imgproc_queue, 1U);
        if (pmsg != GINT_TO_POINTER(-1) && pmsg) delete pmsg;
    }

    thread = g_thread_new("imgproc_thread", imgproc_thread, this);
    if (thread != nullptr)
    {
        return true;
    }
    return false;
}

gboolean
CImgProc::set_enable_send(gboolean enable)
{
    printf("set_enable_send()+ Video Stream:%s\n", enable ? "Enable" : "Disable");
    ImgProcMsg *pmsg = new ImgProcMsg;
    pmsg->msgid = enable ? IMGPROC_ENABLE : IMGPROC_DISABLE ;
    g_async_queue_push(imgproc_queue, pmsg);
    return true;
}

gboolean
CImgProc::stop()
{
    if (!thread_run)
        return true;
    printf("CImgProc::stop()+\n");
    g_async_queue_push(imgproc_queue, ((gpointer)(glong *)(glong)(-1)));
    thread_run = false;

    (void)g_thread_join(thread);
    printf("CImgProc::stop()-\n");
    return true;
}

int 
CImgProc::kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int 
CImgProc::getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        return r;
    }
    else
    {
        return c;
    }
}
// Uncomment to print timings in milliseconds
// #define LOG_TIMES

gboolean
CImgProc::add_new_user(const string name)
{
    printf("add_new_user()+\n");
    ImgProcMsg *pmsg = new ImgProcMsg;
    pmsg->msgid = IMGPROC_ADDNEW;
    pmsg->name = name;
    g_async_queue_push(imgproc_queue, pmsg);
    return true;
}

using namespace nvinfer1;
using namespace nvuffparser;


gboolean 
CImgProc::init_facenet()
{
//------------
    facenet_data *pfd=(facenet_data*)pfacenet_data;
    gLogger = Logger();
    // Register default TRT plugins (e.g. LRelu_TRT)
    if (!initLibNvInferPlugins(&gLogger, ""))
    {
        return false;
    }

    // USER DEFINED VALUES
    const string uffFile = "../facenetModels/facenet.uff";
    const string engineFile = "../facenetModels/facenet.engine";
    DataType dtype = DataType::kHALF;
    //DataType dtype = DataType::kFLOAT;
    bool serializeEngine = true;
    int batchSize = 1;
    pfd->nbFrames = 0;
    // int videoFrameWidth =1280;
    // int videoFrameHeight =720;
    videoFrameWidth = 640;
    videoFrameHeight = 480;

    pfd->maxFacesPerScene = 8;
    float knownPersonThreshold = 1.;

    // init facenet
    pfd->faceNet = new FaceNetClassifier(gLogger, dtype, uffFile, engineFile, batchSize, serializeEngine,
                                                  knownPersonThreshold, pfd->maxFacesPerScene, videoFrameWidth, videoFrameHeight);
    // init mtCNN
    pfd->mtCNN= new mtcnn(videoFrameHeight, videoFrameWidth);

    
    pfd->outputBbox.reserve(pfd->maxFacesPerScene);

    // get embeddings of known faces
#if 0
    std::vector<struct Paths> paths;
    cv::Mat image;
    getFilePaths("../imgs", paths);
    for (int i = 0; i < paths.size(); i++)
    {
        loadInputImage(paths[i].absPath, image, videoFrameWidth, videoFrameHeight);
        pfd->outputBbox = pfd->mtCNN->findFace(image);
        std::size_t index = paths[i].fileName.find_last_of(".");
        std::string rawName = paths[i].fileName.substr(0, index);
        pfd->faceNet.forwardAddFace(image, pfd->outputBbox, rawName);
        pfd->faceNet.resetVariables();
    }
#else
    vector<tFaceEntity> facelist;
    CMydb db;
    cv::Mat image = cv::Mat(videoFrameHeight, videoFrameWidth, 16);
    if (db.list_faces(&facelist)) {
        for (int i=0;i<facelist.size();i++) {
            image.data = facelist[i].data;
            printf("DB Name:%s\n", facelist[i].name.c_str());
            pfd->outputBbox = pfd->mtCNN->findFace(image);
            pfd->faceNet->forwardAddFace(image, pfd->outputBbox, facelist[i].name);
            pfd->faceNet->resetVariables();
            delete facelist[i].data;
            facelist[i].data=nullptr;
        }
    }
    pfd->outputBbox.clear();

#endif        
    return true;
}

gboolean 
CImgProc::deinit_facenet()
{
    facenet_data *pfd=(facenet_data*)pfacenet_data;
    if (!pfd) return false;
    if (pfd->mtCNN) delete pfd->mtCNN;
    if (pfd->faceNet) delete pfd->faceNet;
    return true;
}

gpointer
CImgProc::imgproc_thread(gpointer data)
{
    CImgProc *pthis = (CImgProc *)data;
    facenet_data *pfd=(facenet_data*)pthis->pfacenet_data;
    eImgProc_Runmode running_mode=pthis->run_mode;

    printf("Video Thread running mode=%d\n", running_mode);
    VideoStreamer *videoStreamer;
    if (running_mode!=IMGPROC_MODE_TESTRUN) 
    {
        if (pfd->cameraStreamer==nullptr) 
        {
            if (pthis->camera_dev == "") 
            {
                bool isCSICam = true;
                pfd->cameraStreamer = new VideoStreamer(0, pthis->videoFrameWidth, pthis->videoFrameHeight, 60, isCSICam);
            }
            else
            {
                printf("Using USB Camera Device : %s\n", pthis->camera_dev.c_str());
                pfd->cameraStreamer = new VideoStreamer(pthis->camera_dev, pthis->videoFrameWidth, pthis->videoFrameHeight);
            }
        }
        videoStreamer=pfd->cameraStreamer;
    }
    else 
    {
        printf("Using Video File : %s\n", pthis->video_file.c_str());
        videoStreamer = new VideoStreamer(pthis->video_file, pthis->videoFrameWidth, pthis->videoFrameHeight);
    }


    // init opencv stuff

    cv::Mat frame;

    //init Bbox and allocate memory for "maxFacesPerScene" faces per scene

    //  if  ((TcpListenPort=OpenTcpListenPort(pthis->tcp_port))==NULL)  // Open TCP Network port
    //    {
    //      printf("OpenTcpListenPortFailed\n");
    //      return(nullptr);
    //    }

    //  clilen = sizeof(cli_addr);
    //  printf("Listening for connections   port=%d\n", pthis->tcp_port);

    //  if  ((TcpConnectedPort=AcceptTcpConnection(TcpListenPort,&cli_addr,&clilen))==NULL)
    //    {
    //      printf("AcceptTcpConnection Failed\n");
    //      return(nullptr);
    //    }

    //  printf("Accepted connection Request\n");

    cv::cuda::GpuMat src_gpu, dst_gpu;
    cv::Mat dst_img;
    // loop over frames with inference
    auto globalTimeStart = chrono::steady_clock::now();

    while (pthis->thread_run)
    {
        videoStreamer->getFrame(frame);
        if (frame.empty())
        {
            std::cout << "Empty frame! Exiting...\n Try restarting nvargus-daemon by "
                         "doing: sudo systemctl restart nvargus-daemon"
                      << std::endl;
            break;
        }
        // Create a destination to paint the source into.
        dst_img.create(frame.size(), frame.type());

        // Push the images into the GPU
        if (running_mode!=IMGPROC_MODE_TESTRUN)
        {
            src_gpu.upload(frame);
            cv::cuda::rotate(src_gpu, dst_gpu, src_gpu.size(), 180, src_gpu.size().width, src_gpu.size().height);
            dst_gpu.download(frame);
        }

        auto startMTCNN = chrono::steady_clock::now();
        pfd->outputBbox = pfd->mtCNN->findFace(frame);
        auto endMTCNN = chrono::steady_clock::now();
        auto startForward = chrono::steady_clock::now();
        pfd->faceNet->forward(frame, pfd->outputBbox);
        auto endForward = chrono::steady_clock::now();
        auto startFeatM = chrono::steady_clock::now();
        pfd->faceNet->featureMatching(frame);
        auto endFeatM = chrono::steady_clock::now();
        pfd->faceNet->resetVariables();

        // if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  break;

        // printf("send jpg\n");
        if (pthis->enable_send && pthis->main_queue)
        {
            MyMsg *pmsg = new MyMsg;
            pmsg->msgid = MYMSG_FRAME;
            pmsg->mat = frame.clone(); //Mat copy
            g_async_queue_push(pthis->main_queue, pmsg);
        }

        //cv::imshow("VideoSource", frame);
        pfd->nbFrames++;
        pfd->outputBbox.clear();
        frame.release();

        ImgProcMsg *pmsg = (ImgProcMsg *)g_async_queue_timeout_pop(pthis->imgproc_queue, 10U);
        if (pmsg == GINT_TO_POINTER(-1))
        {
            printf("Exit imgproc.......\n");
            break;
        }
        else if (pmsg)
        {
            switch (pmsg->msgid)
            {
            case IMGPROC_ENABLE:
                pthis->enable_send = true;
                break;
            case IMGPROC_DISABLE:
                pthis->enable_send = false;
                break;
            case IMGPROC_ADDNEW:
            {
                printf("MSG:%d  ADDNEW name=%s\n", pmsg->msgid, pmsg->name.c_str());

                auto dTimeStart = chrono::steady_clock::now();
                videoStreamer->getFrame(frame);
                // Create a destination to paint the source into.
                dst_img.create(frame.size(), frame.type());

                // Push the images into the GPU
                src_gpu.upload(frame);
                cv::cuda::rotate(src_gpu, dst_gpu, src_gpu.size(), 180, src_gpu.size().width, src_gpu.size().height);
                dst_gpu.download(frame);

                pfd->outputBbox = pfd->mtCNN->findFace(frame);

                // if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  break;

                //cv::imshow("VideoSource", frame);
                pfd->faceNet->addNewFace(frame, pfd->outputBbox, pmsg->name);
                auto dTimeEnd = chrono::steady_clock::now();
                globalTimeStart += (dTimeEnd - dTimeStart);
            }
            break;
            }
            delete pmsg;
        }

#ifdef LOG_TIMES
        std::cout << "mtCNN took " << std::chrono::duration_cast<chrono::milliseconds>(endMTCNN - startMTCNN).count() << "ms\n";
        std::cout << "Forward took " << std::chrono::duration_cast<chrono::milliseconds>(endForward - startForward).count() << "ms\n";
        std::cout << "Feature matching took " << std::chrono::duration_cast<chrono::milliseconds>(endFeatM - startFeatM).count() << "ms\n\n";
#endif // LOG_TIMES
    }

    auto globalTimeEnd = chrono::steady_clock::now();

    if (running_mode==IMGPROC_MODE_TESTRUN)  {
        videoStreamer->release();
        sleep(0.5);
        videoStreamer=nullptr;
    }

    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(globalTimeEnd - globalTimeStart).count();
    double seconds = double(milliseconds) / 1000.;
    double fps = pfd->nbFrames / seconds;

    std::cout << "Counted " << pfd->nbFrames << " frames in " << double(milliseconds) / 1000. << " seconds!"
              << " This equals " << fps << "fps.\n";

    return nullptr;
}
