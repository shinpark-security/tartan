#ifndef MY_MSG_H_
#define MY_MSG_H_

#include <glib.h>
#include <opencv2/core.hpp>

typedef enum {
    MYMSG_NONE=0,
    MYMSG_FRAME=1,
    MYMSG_CONTROL=2,
    MYMSG_NET_CONNECTED=3,
    MYMSG_NET_DISCONNECTED=4,
} _e_MSG_ID;

typedef struct {
  _e_MSG_ID msgid;
  int arg1;
  int arg2;
  gpointer pdata;
  gpointer pobj;
  cv::Mat mat;
} MyMsg;


#pragma pack(push, 1)    // 1바이트 크기로 정렬
typedef  struct {
    unsigned char head[4];
    int32_t  size;
} PacketHeader;

typedef  struct {
	PacketHeader hdr;
	unsigned char payload[0];
} Packet;
#pragma pack(pop)


#define PACKET_SIZE 10

#endif /*MY_MSG_H_*/