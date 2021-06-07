
#ifndef COMM_H_
#define COMM_H_

#include <glib.h>
#include "NetworkTCP.h"
#include "TcpSendRecvJpeg.h"
// #include <opencv2/core/core.hpp>



class CComm{
public:
    CComm();
    ~CComm();

    TTcpListenPort    *TcpListenPort;
    TTcpConnectedPort *TcpConnectedPort;

    struct sockaddr_in cli_addr;
    socklen_t          clilen;
    
    int tcp_port;
    gboolean thread_run;
    GThread *thread;
    gboolean start(int port);
    gboolean stop();
    gboolean connection_wait(void) ;
    gboolean send_jpg(const cv::Mat frame);
    static gpointer comm_thread (gpointer data);

};


#endif  /*COMM_H_*/