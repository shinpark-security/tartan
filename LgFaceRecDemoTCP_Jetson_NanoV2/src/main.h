
#ifndef MAIN_H_
#define MAIN_H_

#include <glib.h>
#include "comm.h"
#include "auth.h"
#include "imgproc.h"

typedef struct {
    CComm *pcom;
    CImgProc *pimgproc;
    gboolean thread_run;
    GThread *main_thread;
    GAsyncQueue *queue;
    GMainLoop *mainloop;
    int tcp_port;
    int tls_port;
    static gpointer service_main (gpointer data);
} tServiceData;

#endif /*MAIN_H_*/