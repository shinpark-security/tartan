#ifndef IMGPROC_H_
#define IMGPROC_H_

#include <glib.h>
#include <string>

#define MAX_NAME_LENGTH 20

typedef enum
{
    IMGPROC_NONE = 0,
    IMGPROC_ADDNEW = 1,
    IMGPROC_ENABLE = 2,
    IMGPROC_DISABLE = 3,
} _e_IMGPROC_MSG_ID;

enum eImgProc_Runmode {
    IMGPROC_MODE_NONE=0,
    IMGPROC_MODE_RUN,
    IMGPROC_MODE_LEARNING,
    IMGPROC_MODE_TESTRUN,
};

typedef struct
{
    _e_IMGPROC_MSG_ID msgid;
    string name;
    int nshots;
    int n;
} ImgProcMsg;

class CImgProc
{
public:
    CImgProc(GAsyncQueue *q);
    CImgProc(GAsyncQueue *q, string dev);
    ~CImgProc();
    gboolean thread_run;
    GThread *thread;
    gboolean start(eImgProc_Runmode mode=IMGPROC_MODE_RUN);
    gboolean stop();
    int kbhit();
    int getch();
    gboolean set_enable_send(gboolean enable);
    gboolean add_new_user(const string name, int nshot, int n);
    static gpointer imgproc_thread(gpointer data);
    string camera_dev;
    gboolean init_facenet();
    gboolean deinit_facenet();
    Logger gLogger;
    gpointer pfacenet_data;
    eImgProc_Runmode run_mode;
    string video_file;

private:
    gboolean enable_send;
    GAsyncQueue *main_queue;
    GAsyncQueue *imgproc_queue;
    int videoFrameWidth;
    int videoFrameHeight;

};

#endif /*IMGPROC_H_*/