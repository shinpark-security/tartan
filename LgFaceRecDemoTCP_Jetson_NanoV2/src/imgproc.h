#ifndef IMGPROC_H_
#define IMGPROC_H_

#include <glib.h>


class CImgProc {
public:
    CImgProc();
    ~CImgProc();
    gboolean thread_run;
    GThread *thread;
    gboolean start();
    gboolean stop();

    static gpointer imgproc_thread (gpointer data);
}


#endif /*IMGPROC_H_*/