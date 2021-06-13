#ifndef IMGPROC_H_
#define IMGPROC_H_

#include <glib.h>
#include <string>

#define MAX_NAME_LENGTH 20

typedef enum
{
    IMGPROC_NONE = 0,
    IMGPROC_ADDNEW = 1,
} _e_IMGPROC_MSG_ID;

typedef struct
{
    _e_IMGPROC_MSG_ID msgid;
    string name;
} ImgProcMsg;

class CImgProc
{
public:
    CImgProc(GAsyncQueue *q);
    CImgProc(GAsyncQueue *q, string dev);
    ~CImgProc();
    gboolean thread_run;
    GThread *thread;
    gboolean start();
    gboolean stop();
    int kbhit();
    int getch();
    gboolean set_enable_send(gboolean enable);
    gboolean add_new_user(const string name);
    static gpointer imgproc_thread(gpointer data);
    string camera_dev;

private:
    gboolean enable_send;
    GAsyncQueue *main_queue;
    GAsyncQueue *imgproc_queue;
};

#endif /*IMGPROC_H_*/