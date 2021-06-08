#pragma once

#include <string>
#include "protocol.pb.h"

using namespace std;

class ProtoBuff {
private:
	protocol_msg::ProtocolMsg protocolMsg;
public:
	void setMsgType(const int type);
	void setUserId(const string id);
	void setUserPw(const string password);
	void setAuthority(const int authority);
	void setOperationMode(const int mode);
	void setNumOfImage(const int numOfImage);
	void setImage(const int imgSize, const char* imgData);
	unsigned char* serializeToArray();
	size_t getSize();

	std::string deSerialize(int* msgType, string& userId, string& passWord, int* authority, int* operationMode, int* numImage, int* imgSize, const unsigned char* serializedBuffer, const int serializedBufferSize);
};