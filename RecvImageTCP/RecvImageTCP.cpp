//------------------------------------------------------------------------------------------------
// File: RecvImageTCP.cpp
// Project: LG Exec Ed Program
// Versions:
// 1.0 April 2017 - initial version
// This program receives a jpeg image via a TCP Stream and displays it. 
//----------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "NetworkTCP.h"
#include "TcpSendRecvJpeg.h"


using namespace cv;
using namespace std;
//----------------------------------------------------------------
// main - This is the main program for the RecvImageUDP demo 
// program  contains the control loop
//-----------------------------------------------------------------
#pragma pack(push, 1)    // 1바이트 크기로 정렬
typedef  struct {
    char head[4];
    int  size;
} PacketHeader;

typedef  struct {
	PacketHeader hdr;
	char payload[0];
} Packet;
#pragma pack(pop)


int main(int argc, char *argv[])
{
 TTcpConnectedPort *TcpConnectedPort=NULL;
 bool retvalue;

 char user_acc[40];
 char user_pass[40];
 printf("account:");
 fgets(user_acc,20, stdin);
 printf("password:");
 fgets(user_pass,20, stdin);

 for (int i=0;i<strlen(user_acc);i++)
  if (user_acc[i]<32) user_acc[i]=0;
 for (int i=0;i<strlen(user_pass);i++)
  if (user_pass[i]<32) user_pass[i]=0;

  printf("sizeof PacketHeader=%d\n", sizeof(PacketHeader));
  printf("sizeof packet=%d\n", sizeof(Packet));
  Packet *pkt=(Packet*)new char[1024];
  pkt->hdr.head[0]='S';
  pkt->hdr.head[1]='B';
  pkt->hdr.head[2]='1';
  pkt->hdr.head[3]='T';
  pkt->hdr.size=sizeof(PacketHeader)+40;
  pkt->payload[0]='1';
  char *name=&pkt->payload[1];
  char *passwd=&pkt->payload[21];
  snprintf(name,20,user_acc);
  snprintf(passwd,20,user_pass);
  printf("name=[%s]\n",name);
  printf("passwd=[%s]\n",passwd);
  bool tls_mode = 0;


   if (argc !=4) 
    {
       fprintf(stderr,"usage %s hostname port mode\n", argv[0]);
       exit(0);
    }

   if(!strncmp(argv[3],"1",1)) {
	   tls_mode = true;
   }

   if(tls_mode == true) {
	   wolfSSL_Init();
   }

   if(tls_mode == true) {
	  printf("OpenTcpConnection TLS\n");
	   if  ((TcpConnectedPort=OpenTcpConnectionTLS(argv[1],argv[2]))==NULL)  // Open UDP Network port
	   {
		   printf("OpenTcpConnection\n");
		   return(-1); 
	   }
   } else {
	  printf("OpenTcpConnection non-TLS\n");
	   if  ((TcpConnectedPort=OpenTcpConnection(argv[1],argv[2]))==NULL)  // Open UDP Network port
	   {
		   printf("OpenTcpConnection\n");
		   return(-1); 
	   }
   }

 
  printf("sizeof total real packet size=%d\n", pkt->hdr.size);

  if(tls_mode == true) {
	  WriteDataTcpTLS(TcpConnectedPort, (unsigned char*)pkt, pkt->hdr.size);
  } else {
	  WriteDataTcp(TcpConnectedPort, (unsigned char*)pkt, pkt->hdr.size);
  }

  delete pkt;

  namedWindow( "Server", WINDOW_AUTOSIZE );// Create a window for display.
 
  Mat Image;
  do {
	  if(tls_mode == true) {
		  retvalue=TcpRecvImageAsJpegTLS(TcpConnectedPort,&Image);
	  } else {
		  retvalue=TcpRecvImageAsJpeg(TcpConnectedPort,&Image);
	  }

	  if( retvalue) imshow( "Server", Image ); // If a valid image is received then display it
	  else break;

  } while (waitKey(10) != 'q'); // loop until user hits quit

  if(tls_mode == true) {
	  CloseTcpConnectedPort(&TcpConnectedPort); // Close network port;
  } else {
	  CloseTcpConnectedPort(&TcpConnectedPort); // Close network port;
  }

  if(tls_mode == true) {
	  wolfSSL_Cleanup();
  }
  return 0; 
}
//-----------------------------------------------------------------
// END main
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// END of File
//-----------------------------------------------------------------
