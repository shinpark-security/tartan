
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

CImgProc::CImgProc(){
  thread_run=true;
  main_queue=nullptr;
  imgproc_queue=g_async_queue_new ();
}

CImgProc::~CImgProc(){
  g_async_queue_unref (imgproc_queue);
}

gboolean
CImgProc::start(GAsyncQueue *q){
  main_queue=q;
  thread = g_thread_new ("imgproc_thread", imgproc_thread, this);
  if (thread != nullptr)  {
      return true;
  }
  return false;
}

gboolean 
CImgProc::set_enable_send(gboolean enable)
{
  enable_send=enable;
  return true;
}


gboolean
CImgProc::stop(){
  if (!thread_run) return true;
  g_async_queue_push (imgproc_queue, ((gpointer) (glong*) (glong)(-1)) );
  thread_run=false;

  (void)g_thread_join(thread);
  return true;
}

int
CImgProc::kbhit()
{
    struct timeval tv = { 0L, 0L };
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
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
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
  pmsg->msgid=IMGPROC_ADDNEW;
  pmsg->name=name;
  g_async_queue_push (imgproc_queue, pmsg ); 
  return true;
}
 

using namespace nvinfer1;
using namespace nvuffparser;

gpointer 
CImgProc::imgproc_thread (gpointer data)
{
  CImgProc *pthis=(CImgProc *)data;
  bool               UseCamera=false;

//    if (argc==2) UseCamera=true;
   UseCamera=true;


    Logger gLogger = Logger();
    // Register default TRT plugins (e.g. LRelu_TRT)
    if (!initLibNvInferPlugins(&gLogger, "")) { return nullptr; }

    // USER DEFINED VALUES
    const string uffFile="../facenetModels/facenet.uff";
    const string engineFile="../facenetModels/facenet.engine";
    DataType dtype = DataType::kHALF;
    //DataType dtype = DataType::kFLOAT;
    bool serializeEngine = true;
    int batchSize = 1;
    int nbFrames = 0;
   // int videoFrameWidth =1280;
   // int videoFrameHeight =720;
    int videoFrameWidth = 640;
    int videoFrameHeight = 480;

    int maxFacesPerScene = 8;
    float knownPersonThreshold = 1.;
    bool isCSICam = true;

    // init facenet
    FaceNetClassifier faceNet = FaceNetClassifier(gLogger, dtype, uffFile, engineFile, batchSize, serializeEngine,
            knownPersonThreshold, maxFacesPerScene, videoFrameWidth, videoFrameHeight);
    
    VideoStreamer *videoStreamer;

    // init opencv stuff
    if (UseCamera)  videoStreamer = new VideoStreamer(0, videoFrameWidth, videoFrameHeight, 60, isCSICam);
    // else videoStreamer = new VideoStreamer(argv[2], videoFrameWidth, videoFrameHeight);



    cv::Mat frame;

    // init mtCNN
    mtcnn mtCNN(videoFrameHeight, videoFrameWidth);

    //init Bbox and allocate memory for "maxFacesPerScene" faces per scene
    std::vector<struct Bbox> outputBbox;
    outputBbox.reserve(maxFacesPerScene);

    // get embeddings of known faces
    std::vector<struct Paths> paths;
    cv::Mat image;
    getFilePaths("../imgs", paths);
    for(int i=0; i < paths.size(); i++) {
        loadInputImage(paths[i].absPath, image, videoFrameWidth, videoFrameHeight);
        outputBbox = mtCNN.findFace(image);
        std::size_t index = paths[i].fileName.find_last_of(".");
        std::string rawName = paths[i].fileName.substr(0,index);
        faceNet.forwardAddFace(image, outputBbox, rawName);
        faceNet.resetVariables();
    }
    outputBbox.clear();


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
    
    while (pthis->thread_run) {

        videoStreamer->getFrame(frame);
        if (frame.empty()) {
            std::cout << "Empty frame! Exiting...\n Try restarting nvargus-daemon by "
                         "doing: sudo systemctl restart nvargus-daemon" << std::endl;
            break;
        }
       // Create a destination to paint the source into.
       dst_img.create(frame.size(), frame.type());

      // Push the images into the GPU
      if (UseCamera)
        {
         src_gpu.upload(frame);
         cv::cuda::rotate(src_gpu, dst_gpu, src_gpu.size(), 180, src_gpu.size().width, src_gpu.size().height);
         dst_gpu.download(frame);
        }

        auto startMTCNN = chrono::steady_clock::now();
        outputBbox = mtCNN.findFace(frame);
        auto endMTCNN = chrono::steady_clock::now();
        auto startForward = chrono::steady_clock::now();
        faceNet.forward(frame, outputBbox);
        auto endForward = chrono::steady_clock::now();
        auto startFeatM = chrono::steady_clock::now();
        faceNet.featureMatching(frame);
        auto endFeatM = chrono::steady_clock::now();
        faceNet.resetVariables();
        
        // if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  break;

        if (pthis->enable_send && pthis->main_queue) {
          MyMsg *pmsg = new MyMsg;
          pmsg->msgid=MYMSG_FRAME;
          pmsg->mat=frame.clone(); //Mat copy
          g_async_queue_push (pthis->main_queue, pmsg );
        }

        //cv::imshow("VideoSource", frame);
        nbFrames++;
        outputBbox.clear();
        frame.release();


        ImgProcMsg* pmsg = (ImgProcMsg*) g_async_queue_timeout_pop  (pthis->imgproc_queue, 10U);
        if (pmsg == GINT_TO_POINTER (-1)) {
          printf("Exit imgproc.......\n");
          break;
        } else if (pmsg) {
          switch(pmsg->msgid) {
          case IMGPROC_ADDNEW: {
              printf("MSG:%d  ADDNEW name=%s\n",pmsg->msgid, pmsg->name.c_str());

              auto dTimeStart = chrono::steady_clock::now();
              videoStreamer->getFrame(frame);
              // Create a destination to paint the source into.
              dst_img.create(frame.size(), frame.type());

              // Push the images into the GPU
              src_gpu.upload(frame);
              cv::cuda::rotate(src_gpu, dst_gpu, src_gpu.size(), 180, src_gpu.size().width, src_gpu.size().height);
              dst_gpu.download(frame);

              outputBbox = mtCNN.findFace(frame);

              // if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  break;


              //cv::imshow("VideoSource", frame);
              faceNet.addNewFace(frame, outputBbox, pmsg->name);
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
        #endif  // LOG_TIMES
    }
  
    auto globalTimeEnd = chrono::steady_clock::now();


    videoStreamer->release();

    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(globalTimeEnd-globalTimeStart).count();
    double seconds = double(milliseconds)/1000.;
    double fps = nbFrames/seconds;

    std::cout << "Counted " << nbFrames << " frames in " << double(milliseconds)/1000. << " seconds!" <<
              " This equals " << fps << "fps.\n";

    return nullptr;
}

