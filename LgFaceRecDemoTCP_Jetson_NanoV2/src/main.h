
#ifndef MAIN_H_
#define MAIN_H_

#include <glib.h>
#include "comm.h"
#include "auth.h"
#include "imgproc.h"
#include <list>
using namespace std;


enum SystemState {
    SS_READY=0,
    SS_LOGIN,
    SS_LOGIN_OK,
    SS_LOGIN_NOK,
    SS_RUN,
    SS_LEARN_START,
    SS_LEARN,
    SS_LEARN_DONE,
    SS_TESTRUN_START,
    SS_TESTRUN,
    SS_TESTRUN_DONE,
};

typedef struct {
    // list <CComm *> list_com;
    CComm *pcom;
    CComm *pcom_tls;
    CImgProc *pimgproc;
    gboolean thread_run;
    GThread *main_thread;
    GAsyncQueue *queue;
    GMainLoop *mainloop;
    SystemState sstate;
    static gpointer service_main (gpointer data);
} tServiceData;







#endif /*MAIN_H_*/