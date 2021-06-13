//------------------------------------------------------------------------------------------------
// File: RecvImageTCP.cpp
// Project: LG Exec Ed Program
// Versions:
// 1.0 April 2017 - initial version
// This program receives a jpeg image via a TCP Stream and displays it.
//----------------------------------------------------------------------------------------------
#include "MyProtocol.h"
#include "NetworkTCP.h"
#include "ProtocolManager.h"
#include "TcpSendRecvJpeg.h"
#include "protocolLogin.pb.h"


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
using namespace chrono;

using namespace cv;
using namespace std;
//----------------------------------------------------------------
// main - This is the main program for the RecvImageUDP demo
// program  contains the control loop
//-----------------------------------------------------------------
#define PACKET_MAX_BUFFER_SIZE (1024 * 1024)
bool get_a_packet(TTcpConnectedPort *TcpConnectedPort, unsigned char *buff,  Mat *pImage, bool tls_mode) ;
bool send_packet(TTcpConnectedPort *TcpConnectedPort, CBaseProtocol &protocol, bool tls_mode);

int main(int argc, char *argv[]) {
  TTcpConnectedPort *TcpConnectedPort = NULL;
  bool retvalue;

  char user_acc[40];
  char user_pass[40];
  printf("account:");
  fgets(user_acc, 20, stdin);
  printf("password:");
  fgets(user_pass, 20, stdin);

  for (int i = 0; i < strlen(user_acc); i++)
    if (user_acc[i] < 32)
      user_acc[i] = 0;
  for (int i = 0; i < strlen(user_pass); i++)
    if (user_pass[i] < 32)
      user_pass[i] = 0;

  CProtocolManager proto_man;
  CLoginProtocol login(user_acc, user_pass);
  size_t leng = 0;
  unsigned char *pkt = proto_man.make_packet(login, &leng);

  bool tls_mode = 0;

  if (argc != 4) {
    fprintf(stderr, "usage %s hostname port mode\n", argv[0]);
    exit(0);
  }

  if (!strncmp(argv[3], "1", 1)) {
    tls_mode = true;
  }

  if (tls_mode == true) {
    wolfSSL_Init();
  }

  if (tls_mode == true) {
    printf("OpenTcpConnection TLS\n");
    if ((TcpConnectedPort = OpenTcpConnectionTLS(argv[1], argv[2])) ==
        NULL) // Open UDP Network port
    {
      printf("OpenTcpConnection\n");
      return (-1);
    }
  } else {
    printf("OpenTcpConnection non-TLS\n");
    if ((TcpConnectedPort = OpenTcpConnection(argv[1], argv[2])) ==
        NULL) // Open UDP Network port
    {
      printf("OpenTcpConnection\n");
      return (-1);
    }
  }

  printf("sizeof total real packet size=%zd\n", leng);

  if (tls_mode == true) {
    WriteDataTcpTLS(TcpConnectedPort, pkt, leng);
  } else {
    WriteDataTcp(TcpConnectedPort, pkt, leng);
  }

  delete pkt;

  namedWindow("Server", WINDOW_AUTOSIZE); // Create a window for display.

    Mat Image;
    unsigned char key=0;
    unsigned char *buff=new (std::nothrow) unsigned char[PACKET_MAX_BUFFER_SIZE];	/* receive buffer */   
    if (buff==NULL) return false;
    do {
        // if (tls_mode == true) {
        //     retvalue = TcpRecvImageAsJpegTLS(TcpConnectedPort, &Image);
        // } else {
        //     retvalue = TcpRecvImageAsJpeg(TcpConnectedPort, &Image);
        // }

        // // printf("ret value=%d\n", retvalue);
        // if (retvalue)
        //     imshow("Server", Image); // If a valid image is received then display it

        get_a_packet(TcpConnectedPort, buff, &Image, tls_mode);

        key=waitKey(10);
        switch(key) {
            case 'd': {
                CLoginProtocol login(user_acc, user_pass);
                send_packet(TcpConnectedPort, login, tls_mode);
            }
            break;
            case 's': {
                CServerSettingProtocol ctl(protocol_msg::ServerSetting::CAM_START);
                send_packet(TcpConnectedPort, ctl, tls_mode);
            }
            break;
            case 'S': {
                CServerSettingProtocol ctl(protocol_msg::ServerSetting::CAM_STOP);
                send_packet(TcpConnectedPort, ctl, tls_mode);
            }
            break;
            case '1': {
                CControlModeProtocol mode(protocol_msg::ControlMode::RUN);
                send_packet(TcpConnectedPort, mode, tls_mode);
              }
              break;
            case '2': {
                CControlModeProtocol mode(protocol_msg::ControlMode::LEARNING);
                send_packet(TcpConnectedPort, mode, tls_mode);
              }
              break;
            case '3': {
                CControlModeProtocol mode(protocol_msg::ControlMode::TESTRUN);
                send_packet(TcpConnectedPort, mode, tls_mode);
              }
              break;
            break;
        }

    } while (key != 'q'); // loop until user hits quit
    delete [] buff;

  if (tls_mode == true) {
    CloseTcpConnectedPort(&TcpConnectedPort); // Close network port;
  } else {
    CloseTcpConnectedPort(&TcpConnectedPort); // Close network port;
  }

  if (tls_mode == true) {
    wolfSSL_Cleanup();
  }
  return 0;
}


bool send_packet(TTcpConnectedPort *TcpConnectedPort, CBaseProtocol &protocol, bool tls_mode)
{
    CProtocolManager proto_man;
    size_t leng = 0;
    unsigned char *pkt = proto_man.make_packet(protocol, &leng);
    if (tls_mode == true) {
        WriteDataTcpTLS(TcpConnectedPort, pkt, leng);
    } else {
        WriteDataTcp(TcpConnectedPort, pkt, leng);
    }
    delete pkt;
    return true;
}


bool get_a_packet(TTcpConnectedPort *TcpConnectedPort, unsigned char *buff, Mat *pImage, bool tls_mode) 
{
    ssize_t ret=0;
    if (tls_mode == true) 
        ret = ReadDataTcpTLS(TcpConnectedPort,buff,PACKET_MAX_BUFFER_SIZE);
    else
        ret = ReadDataTcp(TcpConnectedPort,buff,PACKET_MAX_BUFFER_SIZE);
    if (ret <= PACKET_MAX_BUFFER_SIZE && ret > 0) 
    {
        CProtocolManager prot_man;
        CBaseProtocol *pbase=dynamic_cast<CBaseProtocol*>(prot_man.parse_packet((MyPacket*)buff));
        if (pbase) {
            printf("Protocol type=%d\n", pbase->msg_type);
            switch(pbase->msg_type)
            {
                case MSG_IMAGE:
                {
                    printf("MsgReq::MSG_IMAGE\n");
                    CImageProtocol *img_pkt=dynamic_cast<CImageProtocol*>(pbase);
                    cv::imdecode(cv::Mat(img_pkt->msg.img_size(), 1, CV_8UC1, (uchar*)img_pkt->msg.img_data().c_str()), cv::IMREAD_COLOR, pImage );
                    // pImage->data = (uchar*)img_pkt->msg.img_data().c_str();
                    if (!(*pImage).empty()) imshow("Server", *pImage); // If a valid image is received then display it
                }
                break;
            }
        } else {
            printf("parsing error..\n");
        }
    }
    else {
        return false;
    }
    return true;
}
//-----------------------------------------------------------------
// END main
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// END of File
//-----------------------------------------------------------------
