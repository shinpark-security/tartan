
#include "comm.h"


CComm::CComm() {
}

CComm::~CComm() {
    stop();
}


gboolean
CComm::connection_wait(void) {
    clilen = sizeof(cli_addr);
    printf("Listening for connections   port=%d\n", tcp_port);

    if  ((TcpConnectedPort=AcceptTcpConnection(TcpListenPort,&cli_addr,&clilen))==NULL)
    {  
        printf("AcceptTcpConnection Failed\n");
        return false;
    }

    printf("Accepted connection Request\n");
    return true;
}

gboolean
CComm::send_jpg(const cv::Mat frame) {

    if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  return false;
    return true;
}

gpointer CComm::comm_thread (gpointer data) {
    printf("comm_thread()+");
    CComm *pcom=(CComm*)data;
    if  ((pcom->TcpListenPort=OpenTcpListenPort(pcom->tcp_port))==NULL)  // Open TCP Network port
    {
        printf("OpenTcpListenPortFailed\n");
        return(nullptr); 
    }    

    while(pcom->thread_run) {
        sleep(1);
        printf("comm thread...\n");
    }
    printf("comm_thread()-");
}

gboolean
CComm::start(int port) {
    printf("comm-start()+\n");
    tcp_port=port;
    thread_run=true;

    thread = g_thread_new ("comm_thread", comm_thread, this);
    if (thread != nullptr)  {
        return true;
    }
    return false;
}

gboolean
CComm::stop() {
    thread_run=false;
    (void)g_thread_join(thread);
    return true;
}