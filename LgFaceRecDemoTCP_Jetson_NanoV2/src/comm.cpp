#include "common.h"
#include "comm.h"
#include "mymsg.h"
#include <sys/socket.h> 
#include <sys/poll.h> 

CComm::CComm() {
    tcp_connected=false;
    g_mutex_init(&lock);
 
}

CComm::~CComm() {
    stop();
    g_mutex_clear(&lock);
}


gboolean
CComm::connection_wait(void) {
    clilen = sizeof(cli_addr);
    printf("Listening for connections   port=%d\n", tcp_port);

	if  ((TcpConnectedPort=AcceptTcpConnectionTLS(TcpListenPort,&cli_addr,&clilen))==NULL)
	{  
		printf("AcceptTcpConnection Failed\n");
		return false;
	}

    printf("Accepted connection Request\n");
    return true;
}

gboolean
CComm::send_jpg(const cv::Mat frame) {

    gboolean ret=true;
    if (!tcp_connected) return false;
    g_mutex_lock (&lock);
    int sents=0;
    try {
        // printf("sending...\n");
        sents=TcpSendImageAsJpegTLS(TcpConnectedPort,frame);
        // printf("sent...%d\n",sents);
        if (sents<0)  {
            tcp_connected=false;
            ret=false;
        }    
    }
    catch (int ex) {
        printf("TcpSendImageAsJpeg Exception...%d\n",ex);
        tcp_connected=ret;
        CloseTcpConnectedPortTLS(&TcpConnectedPort);
        ret=false;
    }   
    g_mutex_unlock (&lock);

    return ret;
}

gboolean
CComm::send_response(const unsigned char *buff, size_t len) {

    gboolean ret=true;
    if (!tcp_connected) return false;
    g_mutex_lock (&lock);
    int sents=0;
    try {
        // printf("sending...\n");
        sents=WriteDataTcpTLS(TcpConnectedPort,(unsigned char*)buff,(size_t)len);
        // printf("sent...%d\n",sents);
        if (sents<0)  {
            tcp_connected=false;
            ret=false;
        }    
    }
    catch (int ex) {
        printf("send_response Exception...%d\n",ex);
        tcp_connected=ret;
        CloseTcpConnectedPortTLS(&TcpConnectedPort);
        ret=false;
    }   
    g_mutex_unlock (&lock);

    return ret;
}
gpointer 
CComm::comm_thread (gpointer data) {
    printf("comm_thread()+");
    CComm *pcom=(CComm*)data;
    if  ((pcom->TcpListenPort=OpenTcpListenPortTLS(pcom->tcp_port))==NULL)  // Open TCP Network port
    {
        printf("OpenTcpListenPortTLSFailed\n");
        return(nullptr); 
    }

    while(pcom->thread_run) {


        if (!pcom->tcp_connected) {
            printf("wait for connection..\n");
            try {
                if (pcom->connection_wait()) {
                    pcom->tcp_connected=true;
                    printf("Connected.........................\n");
                }
            }
            catch (int ex) {
                    printf("connection_wait Exception...%d\n",ex);
                    pcom->tcp_connected=false;
                    CloseTcpConnectedPortTLS(&pcom->TcpConnectedPort);
                    continue;
            }
        } else {
            // printf("check...\n");
            struct pollfd pfd;
            pfd.fd = pcom->TcpConnectedPort->ConnectedFd;
            pfd.events = POLLIN | POLLHUP | POLLRDNORM;
            pfd.revents = 0;
            unsigned char buffer[PACKET_SIZE];
            int ret=0;

            if (pfd.revents == 0) {
                // g_mutex_lock (&pcom->lock);
                memset(buffer,0,PACKET_SIZE);
               
                int pollret=0;
                try {
                    pollret=poll(&pfd, 1, 100);  // call poll with a timeout of 100 ms
                }
                catch (int ex) {
                    printf("Poll Exception...%d\n",ex);
                    pcom->tcp_connected=false;
                    CloseTcpConnectedPortTLS(&pcom->TcpConnectedPort);
                }
                if ( pollret > 0) {
                    try {
                        ret=wolfSSL_recv(pcom->TcpConnectedPort->ssl, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);
                        if (ret==0) {
                            printf("Check ERROR...............\n");
                            pcom->tcp_connected=false;
                            CloseTcpConnectedPort(&pcom->TcpConnectedPort);
                        }
                        if (ret>0) {
                            ret=ReadDataTcpTLS(pcom->TcpConnectedPort,buffer,PACKET_SIZE);
                            // printf("bytes=%d, data=[%10s]\n", ret, buffer);     
                            if (pcom->main_queue) {
                                MyMsg *pmsg = new MyMsg;
                                pmsg->msgid=MYMSG_CONTROL;
                                pmsg->pdata=(gpointer)new char[ret];
                                memcpy(pmsg->pdata,buffer,ret);
                                g_async_queue_push (pcom->main_queue, pmsg );
                            }                            
                        }
                    }
                    catch (int ex) {
                        printf("Exception...%d\n",ex);
                        pcom->tcp_connected=false;
                        CloseTcpConnectedPortTLS(&pcom->TcpConnectedPort);

                    }
                    
                }
                // g_mutex_unlock (&pcom->lock);
            }

        }
        sleep(0.1);
        // printf("comm thread...\n");
    }
    printf("comm_thread()-");
	CloseTcpListenPortTLS(&pcom->TcpListenPort);
}

gboolean
CComm::start(int port, GAsyncQueue *q) {
    printf("comm-start()+\n");
    main_queue=q;
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
    if (!thread_run) return true;
    thread_run=false;
    (void)g_thread_join(thread);
    return true;
}
