#include "common.h"
#include "comm.h"
#include "mymsg.h"
#include <sys/socket.h> 
#include <sys/poll.h> 

// open_tcp_listen_port_func CComm::open_tcp_listen_port;
// close_tcp_listen_port_func CComm::close_tcp_listen_port;
// accept_tcp_connection_func CComm::accept_tcp_connection;
// close_tcp_connected_port_func CComm::close_tcp_connected_port;
// read_data_tcp_func CComm::read_data_tcp;
// write_data_tcp_func CComm::write_data_tcp;
// tcp_send_image_as_jpeg_func CComm::tcp_send_image_as_jpeg;

CComm::CComm(gboolean tls, int port, GAsyncQueue *q) {
    tcp_connected=false;
    tls_mode=tls;
    main_queue=q;
    tcp_port=port;
    thread_run=false;
    TcpConnectedPort=nullptr;
    thread_pause=false;
    g_mutex_init(&lock);
    if (tls) {
        printf("TLS MODE\n");
        open_tcp_listen_port=OpenTcpListenPortTLS;
        close_tcp_listen_port=CloseTcpListenPortTLS;
        accept_tcp_connection=AcceptTcpConnectionTLS;
        close_tcp_connected_port=CloseTcpConnectedPortTLS;
        read_data_tcp=ReadDataTcpTLS;
        write_data_tcp=WriteDataTcpTLS;
        tcp_send_image_as_jpeg=TcpSendImageAsJpegTLS;
    } else {
        printf("NON TLS MODE\n");
        open_tcp_listen_port=OpenTcpListenPort;
        close_tcp_listen_port=CloseTcpListenPort;
        accept_tcp_connection=AcceptTcpConnection;
        close_tcp_connected_port=CloseTcpConnectedPort;
        read_data_tcp=ReadDataTcp;
        write_data_tcp=WriteDataTcp;
        tcp_send_image_as_jpeg=TcpSendImageAsJpeg;
    } 
}

CComm::~CComm() {
    stop();
    g_mutex_clear(&lock);
}


gboolean
CComm::start() {
    if (thread_run)
        return true;
    printf("comm-start()+  TLS=%d\n", tls_mode);
    thread_run=true;
    thread = g_thread_new ("comm_thread", comm_thread, this);
    if (thread != nullptr)  {
        return true;
    }
    return false;
}

gboolean
CComm::stop() {
    printf("CComm::stop()+ TLS=%d\n", tls_mode);
    if (!thread_run) return true;
    printf("trying to close the port\n");
    thread_run=false;
    // close(TcpListenPort->ListenFd);
    close_tcp_connected_port(&TcpConnectedPort);
    close_tcp_listen_port(&TcpListenPort);
    printf("closed and wait for thread join.\n");
    (void)g_thread_join(thread);
    printf("thread join finished\n");
    thread=nullptr;
    printf("CComm::stop()-\n");
    return true;
}

gboolean
CComm::pause() {
    printf("CComm::pause()+ TLS=%d\n", tls_mode);
    if (!thread_run) return true;
    thread_pause=true;
    close_tcp_connected_port(&TcpConnectedPort);
    close_tcp_listen_port(&TcpListenPort);
    return true;
}

gboolean
CComm::resume() {
    printf("comm-resume()+  TLS=%d\n", tls_mode);
    if (!thread_run) return true;
    thread_pause=false;
    return true;
}


gboolean
CComm::connection_wait(void) {
    clilen = sizeof(cli_addr);
    printf("Listening for connections   port=%d TLS Mode=%d\n", tcp_port, tls_mode);

	if  ((TcpConnectedPort=accept_tcp_connection(TcpListenPort,&cli_addr,&clilen))==NULL)
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
        sents=tcp_send_image_as_jpeg(TcpConnectedPort,frame);
        // printf("sent...%d\n",sents);
        if (sents<0)  {
            tcp_connected=false;
            ret=false;
        }    
    }
    catch (int ex) {
        printf("tcp_send_image_as_jpeg Exception...%d\n",ex);
        tcp_connected=ret;
        close_tcp_connected_port(&TcpConnectedPort);
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
        sents=write_data_tcp(TcpConnectedPort,(unsigned char*)buff,(size_t)len);
        // printf("sent...%d\n",sents);
        if (sents<0)  {
            tcp_connected=false;
            ret=false;
        }    
    }
    catch (int ex) {
        printf("send_response Exception...%d\n",ex);
        tcp_connected=ret;
        close_tcp_connected_port(&TcpConnectedPort);
        ret=false;
    }   
    g_mutex_unlock (&lock);

    return ret;
}

gboolean 
CComm::send_msg_connected (gboolean connected) {
    printf("Connection Msg sent..TLS=%d\n", tls_mode);
    if (main_queue) {
        MyMsg *pmsg = new MyMsg;
        pmsg->msgid= connected ? MYMSG_NET_CONNECTED : MYMSG_NET_DISCONNECTED;
        pmsg->arg1= tls_mode;
        pmsg->pdata=this;
        g_async_queue_push (main_queue, pmsg );
    }
    return true;
}

gpointer 
CComm::comm_thread (gpointer data) {
    printf("comm_thread()+\n");
    CComm *pcom=(CComm*)data;
    if  ((pcom->TcpListenPort=pcom->open_tcp_listen_port(pcom->tcp_port))==NULL)  // Open TCP Network port
    {
        printf("open_tcp_listen_port Failed\n");
        return(nullptr); 
    }

    while(pcom->thread_run) {
        if (pcom->thread_pause) {
            sleep(1);
            printf("thread paused... TLS=%d\n",pcom->tls_mode);
            continue;
        }

        // printf("comthread:tls=%d", pcom->tls_mode);
        if (!pcom->tcp_connected) {
            printf("wait for connection..\n");
            try {
                if (pcom->connection_wait()) {
                    pcom->tcp_connected=true;
                    pcom->send_msg_connected(true);
                    printf("Connected.........................TLS=%d\n",pcom->tls_mode);
                }
            }
            catch (int ex) {
                    printf("connection_wait Exception...%d.. and.TLS=%d\n",ex,pcom->tls_mode);
                    pcom->tcp_connected=false;
                    pcom->close_tcp_connected_port(&pcom->TcpConnectedPort);
                    pcom->send_msg_connected(false);
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
                    pcom->send_msg_connected(false);
                    pcom->close_tcp_connected_port(&pcom->TcpConnectedPort);
                }
                if ( pollret > 0) {
                    try {
                        if (pcom->tls_mode)
                            ret=wolfSSL_recv(pcom->TcpConnectedPort->ssl, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);
                        else
                            ret=recv(pcom->TcpConnectedPort->ConnectedFd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);
                        if (ret==0) {
                            printf("Check ERROR...............\n");
                            pcom->tcp_connected=false;
                            pcom->send_msg_connected(false);
                            pcom->close_tcp_connected_port(&pcom->TcpConnectedPort);
                        }
                        if (ret>0) {
                            ret=pcom->read_data_tcp(pcom->TcpConnectedPort,buffer,PACKET_SIZE);
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
                        pcom->send_msg_connected(false);
                        pcom->close_tcp_connected_port(&pcom->TcpConnectedPort);
                    }
                    
                }
                // g_mutex_unlock (&pcom->lock);
            }

        }
        sleep(0.1);
        // printf("comm thread...\n");
    }
    printf("comm_thread()-");
	pcom->close_tcp_listen_port(&pcom->TcpListenPort);
}
