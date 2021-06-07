#ifndef MY_MSG_H_
#define MY_MSG_H_

#include <glib.h>
#include <opencv2/core.hpp>

typedef enum {
    MYMSG_NONE=0,
    MYMSG_FRAME=1,
} _e_MSG_ID;

typedef struct {
  _e_MSG_ID msgid;
  int arg1;
  int arg2;
  gpointer pdata;
  cv::Mat mat;
} MyMsg;


#endif /*MY_MSG_H_*/