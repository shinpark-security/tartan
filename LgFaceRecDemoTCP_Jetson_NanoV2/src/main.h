
#ifndef MAIN_H_
#define MAIN_H_

#include <glib.h>


typedef struct {
    GThread *thread;
    GThread *imgprocess_thread;
    GThread *comm_thread;

    GAsyncQueue *queue;
    GMainLoop *mainloop;
    gboolean thread_run;
    int tcp_port;
    int tls_port;
    static gpointer service_main (gpointer data);
} tServiceData;

#endif /*MAIN_H_*/