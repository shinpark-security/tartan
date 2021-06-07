
#include "comm.h"
#include "mymsg.h"
#include <sys/socket.h> 
#include <sys/poll.h> 

CComm::CComm() {
    tcp_connected=false;
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

    if (!tcp_connected) return false;

    if (TcpSendImageAsJpeg(TcpConnectedPort,frame)<0)  {
        tcp_connected=false;
        return false;
    }

    return true;
}

gpointer 
CComm::comm_thread (gpointer data) {
    printf("comm_thread()+");
    CComm *pcom=(CComm*)data;
    if  ((pcom->TcpListenPort=OpenTcpListenPort(pcom->tcp_port))==NULL)  // Open TCP Network port
    {
        printf("OpenTcpListenPortFailed\n");
        return(nullptr); 
    }    

    while(pcom->thread_run) {


        if (!pcom->tcp_connected) {
            if (pcom->connection_wait()) {
                pcom->tcp_connected=true;
                printf("Connected.........................\n");
            }
        } else {
            printf("check...\n");
// 
            // int retval = getsockopt (pcom->TcpConnectedPort->ConnectedFd, SOL_SOCKET, SO_ERROR, &error, &len);


            // use the poll system call to be notified about socket status changes
            struct pollfd pfd;
            pfd.fd = pcom->TcpConnectedPort->ConnectedFd;
            pfd.events = POLLIN | POLLHUP | POLLRDNORM;
            pfd.revents = 0;
            while (pfd.revents == 0) {
                // call poll with a timeout of 100 ms
                if (poll(&pfd, 1, 100) > 0) {
                    printf("ERROR...............\n");
                    pcom->tcp_connected=false;
                    CloseTcpConnectedPort(&pcom->TcpConnectedPort);

                    // if result > 0, this means that there is either data available on the
                    // socket, or the socket has been closed
                    // char buffer[32];
                    // if (recv(fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
                    //     // if recv returns zero, that means the connection has been closed:
                    //     // kill the child process
                    //     kill(childProcess, SIGKILL);
                    //     waitpid(childProcess, &status, WNOHANG);
                    //     close(fd);
                    //     // do something else, e.g. go on vacation
                    // }
                }
            }

        }
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
    if (!thread_run) return true;
    thread_run=false;
    (void)g_thread_join(thread);
    return true;
}