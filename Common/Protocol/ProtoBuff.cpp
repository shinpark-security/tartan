#include "ProtoBuff.h"

void ProtoBuff::setMsgType(const int type) 
{
	protocolMsg.set_msg_type(type);
}

void ProtoBuff::setUserId(const string id)
{
	protocolMsg.set_user_id(id);
}

void ProtoBuff::setUserPw(const string password)
{
	protocolMsg.set_password(password);
}

void ProtoBuff::setAuthority(const int authority)
{
	protocolMsg.set_authority(authority);
}

void ProtoBuff::setOperationMode(const int mode)
{
	protocolMsg.set_operation_mode(mode);
}

void ProtoBuff::setNumOfImage(const int numOfImage)
{
	protocolMsg.set_num_image(numOfImage);
}

void ProtoBuff::setImage(const int imgSize, const char* imgData)
{
	protocolMsg.set_img_size(imgSize);
	std::string data_str(reinterpret_cast<char const*>(imgData), imgSize);
	protocolMsg.set_img_data(data_str);
}

unsigned char* ProtoBuff::serializeToArray() {
	size_t bufferLength = protocolMsg.ByteSizeLong();
	unsigned char* buffer = new unsigned char[bufferLength];
	bool result = protocolMsg.SerializeToArray(buffer, bufferLength);
	if (true == result) {
		return buffer;
	}
	else {
		// TODO : error Ã³¸®
	}
}

size_t ProtoBuff::getSize() {
	return protocolMsg.ByteSizeLong();
}

std::string ProtoBuff::deSerialize(int* msgType, string& userId, string& passWord, int* authority, int* operationMode, int* numImage, int* imgSize, const unsigned char* serializedBuffer, const int serializedBufferSize) {
	protocolMsg.ParseFromArray(serializedBuffer, serializedBufferSize);
	*operationMode = protocolMsg.operation_mode();
	*imgSize = protocolMsg.img_size();
	
	return protocolMsg.img_data();
}
