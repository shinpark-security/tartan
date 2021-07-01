//------------------------------------------------------------------------------------------------
// File: SendImageTCP.cpp
// Project: LG Exec Ed Program
// Versions:
// 1.0 April 2017 - initial version
// This program Sends a jpeg image From the Camera via a TCP Stream to a remote destination. 
//----------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "NetworkTCP.h"
#include "TcpSendRecvJpeg.h"
#include "../Common/Protocol/MyProtocol.h"

using namespace cv;
using namespace std;


std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}



//----------------------------------------------------------------
// main - This is the main program for the RecvImageUDP demo 
// program  contains the control loop
//---------------------------------------------------------------

int main(int argc, char *argv[])
{

  Mat                image;          // camera image in Mat format 
  TTcpListenPort    *TcpListenPort;
  TTcpConnectedPort *TcpConnectedPort;
  struct sockaddr_in cli_addr;
  socklen_t          clilen;
  int key;

    if (argc !=2) 
    {
       fprintf(stderr,"usage %s port\n", argv[0]);
       exit(0);
    }
/*
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 2 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    //cv::VideoCapture capture("/dev/video0");
    cv::VideoCapture capture(pipeline, cv::CAP_GSTREAMER);
    if(!capture.isOpened()) {
	std::cout<<"Failed to open camera."<<std::endl;
	return (-1);
    }
*/

   if  ((TcpListenPort=OpenTcpListenPort(atoi(argv[1])))==NULL)  // Open UDP Network port
     {
       printf("OpenTcpListenPortFailed\n");
       return(-1); 
     }

    
   clilen = sizeof(cli_addr);
    
   printf("Listening for connections\n");

   if  ((TcpConnectedPort=AcceptTcpConnection(TcpListenPort,&cli_addr,&clilen))==NULL)
     {  
       printf("AcceptTcpConnection Failed\n");
       return(-1); 
     }

   printf("Accepted connection Request\n");

   const int HEADER_SIZE = 10000;
   unsigned char* packetHeader = new (std::nothrow) unsigned char[HEADER_SIZE];

   unsigned int msgSize = 0;
   msgSize = ReadDataTcp(TcpConnectedPort, packetHeader, HEADER_SIZE);
   if (msgSize > 8) {
	   printf("packet size : %d\n", msgSize);
	   int msgType = 0;
	   string userId = "";
	   string userPassword = "";
	   int authority = 0;

	   LoginProtocol* msgBuff = new LoginProtocol();
	   msgBuff->deSerialize(&msgType, userId, userPassword, &authority, &packetHeader[8], msgSize - 8);

	   printf("msgType : %d\n", msgType);
	   printf("userId : %s\n", userId.c_str());
	   printf("userPassword : %s\n", userPassword.c_str());

	   delete msgBuff;
   }

   // SEND Success response message


   delete[] packetHeader;
   /*
    const int HEADER_SIZE = 8;
    unsigned char* packetHeader = new (std::nothrow) unsigned char[HEADER_SIZE];
	if (HEADER_SIZE == ReadDataTcp(TcpConnectedPort, packetHeader, HEADER_SIZE)) {
		unsigned int packetSize;
		memcpy(&packetSize, &packetHeader[4], 4);
		packetSize = ntohl(packetSize);
		printf("packet size : %d\n", packetSize);

		unsigned char* buff;
		buff = new (std::nothrow) unsigned char[packetSize];
		if (packetSize == ReadDataTcp(TcpConnectedPort, buff, packetSize)) {
			int msgType = 0;
			string userId = "";
			string userPassword = "";
			int authority = 0;

			LoginProtocol* msgBuff = new LoginProtocol();
			msgBuff->deSerialize(&msgType, userId, userPassword, &authority, buff, packetSize);

			printf("msgType : %d\n", msgType);
			printf("userId : %s\n", userId.c_str());
			printf("userPassword : %s\n", userPassword.c_str());
		}
		delete(buff);
	}
*/

/*
    unsigned int packetSize;
    if (ReadDataTcp(TcpConnectedPort, (unsigned char*)&packetSize, sizeof(packetSize)) != sizeof(packetSize))
	{
		printf("Get packet size error\n");
		return 0;
	}
	packetSize = ntohl(packetSize);
	printf("packet size : %d\n", packetSize);


	unsigned char* buff;
	buff = new (std::nothrow) unsigned char[packetSize];
	if (packetSize == ReadDataTcp(TcpConnectedPort, buff, packetSize)) {
		int msgType = 0;
		string userId = "";
		string userPassword = "";
		int authority = 0;

		LoginProtocol* msgBuff = new LoginProtocol();
		msgBuff->deSerialize(&msgType, userId, userPassword, &authority, buff, packetSize);

		printf("msgType : %d\n", msgType);
		printf("userId : %s\n", userId.c_str());
		printf("userPassword : %s\n", userPassword.c_str());
	}
*/

   /*

  do
   {
    // wait for a new frame from camera and store it into 'frame'
    capture.read(image);
    // check if we succeeded
    if (image.empty())
    {
      printf("ERROR! blank frame grabbed\n");
      continue;
    }
#if 0	
   cudaRGBA32ToBGR8(image, (uchar3*)rgb_gpu, capture_width, capture_height );      
        
   cv::cuda::GpuMat imgRGB_gpu(imgHeight, capture_height, CV_8UC3, rgb_gpu);                

   std::vector<struct Bbox> detections;
   finder.findFace(imgRGB_gpu, &detections);

   std::vector<cv::Rect> rects;
   std::vector<float*> keypoints;
   num_dets = get_detections(origin_cpu, &detections, &rects, &keypoints);            
#endif	

    // Send processed UDP image
    if (TcpSendImageAsJpegProtocol(TcpConnectedPort,image)<0)  break;   
	key = (waitKey(10) & 0xFF);
	printf("%d\n",key);
   } while (key!= 'q'); // loop until user hits quit
*/
 CloseTcpConnectedPort(&TcpConnectedPort); // Close network port;
 CloseTcpListenPort(&TcpListenPort);  // Close listen port

 return 0; 
}
//-----------------------------------------------------------------
// END main
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// END of File
//-----------------------------------------------------------------
