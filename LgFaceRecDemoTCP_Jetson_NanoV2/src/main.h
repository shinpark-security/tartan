
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
    SS_TESTRUN,
    SS_TESTRUN_DONE,
};

enum SessionPrivilege {
    SP_NONE = -1,
    SP_ADMIN = 0,
    SP_USER1 = 1,
    SP_USER2 = 2,
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
    SessionPrivilege privilege;
    static gpointer service_main (gpointer data);
} tServiceData;







#endif /*MAIN_H_*/