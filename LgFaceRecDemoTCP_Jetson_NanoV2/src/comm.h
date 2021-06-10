
#ifndef COMM_H_
#define COMM_H_

#include <glib.h>
#include <string>
#include "network.h"
#include "NetworkTCP.h"
#include "TcpSendRecvJpeg.h"

typedef TTcpListenPort *(*open_tcp_listen_port_func)(short localport);
typedef void (*close_tcp_listen_port_func)(TTcpListenPort **TcpListenPort);
typedef TTcpConnectedPort *(*accept_tcp_connection_func)(TTcpListenPort *TcpListenPort, 
                    struct sockaddr_in *cli_addr,socklen_t *clilen);
typedef void (*close_tcp_connected_port_func)(TTcpConnectedPort **TcpConnectedPort);
typedef ssize_t (*read_data_tcp_func)(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length);
typedef ssize_t (*write_data_tcp_func)(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length);
typedef int (*tcp_send_image_as_jpeg_func)(TTcpConnectedPort * TcpConnectedPort,cv::Mat Image);


class CComm{
public:
    CComm(gboolean tls, int port, GAsyncQueue *q);
    ~CComm();

    TTcpListenPort    *TcpListenPort;
    TTcpConnectedPort *TcpConnectedPort;

    struct sockaddr_in cli_addr;
    socklen_t          clilen;
    
    int tcp_port;
    gboolean thread_run;
    GThread *thread;
    gboolean tcp_connected;
    gboolean start();
    gboolean stop();
    gboolean connection_wait(void) ;
    gboolean send_jpg(const cv::Mat frame);
    gboolean send_response(const unsigned char *buff, size_t len);

    static gpointer comm_thread (gpointer data);
    gboolean send_msg_connected (gboolean connected);
    open_tcp_listen_port_func open_tcp_listen_port;
    close_tcp_listen_port_func close_tcp_listen_port;
    accept_tcp_connection_func accept_tcp_connection;
    close_tcp_connected_port_func close_tcp_connected_port;
    read_data_tcp_func read_data_tcp;
    write_data_tcp_func write_data_tcp;
    tcp_send_image_as_jpeg_func tcp_send_image_as_jpeg;
    gboolean thread_pause;
    gboolean pause();
    gboolean resume();
    gboolean disconnect();
private:
    GMutex lock;
    GAsyncQueue *main_queue;
    gboolean tls_mode;
};


#endif  /*COMM_H_*/