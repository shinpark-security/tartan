
#ifndef MAIN_H_
#define MAIN_H_

#include <glib.h>
#include "comm.h"
#include "auth.h"
#include "imgproc.h"
#include <list>
using namespace std;

typedef struct {
    // list <CComm *> list_com;
    CComm *pcom;
    CComm *pcom_tls;
    CImgProc *pimgproc;
    gboolean thread_run;
    GThread *main_thread;
    GAsyncQueue *queue;
    GMainLoop *mainloop;
    static gpointer service_main (gpointer data);
} tServiceData;

#endif /*MAIN_H_*/