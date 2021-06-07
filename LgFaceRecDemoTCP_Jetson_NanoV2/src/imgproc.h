#ifndef IMGPROC_H_
#define IMGPROC_H_

#include <glib.h>


class CImgProc {
public:
    CImgProc();
    ~CImgProc();
    gboolean thread_run;
    GThread *thread;
    gboolean start(GAsyncQueue *q);
    gboolean stop();
    int kbhit();
    int getch();
    gboolean set_enable_send(gboolean enable);
    GAsyncQueue *main_queue;

    static gpointer imgproc_thread (gpointer data);
private:
    gboolean enable_send;
};


#endif /*IMGPROC_H_*/