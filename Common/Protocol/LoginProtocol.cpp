#include "LoginProtocol.h"

void LoginProtocol::setMsgType(const int type)
{
	msg.set_msg_type(type);
}

void LoginProtocol::setTimestamp(const int time)
{
	msg.set_timestamp(time);
}

void LoginProtocol::setUserId(const string id)
{
	msg.set_user_id(id);
}

void LoginProtocol::setUserPw(const string password)
{
	msg.set_password(password);
}

void LoginProtocol::setAuthority(const int authority)
{
	msg.set_authority(authority);
}

unsigned char* LoginProtocol::serializeToArray()
{
	size_t bufferLength = msg.ByteSize();
	unsigned char* buffer = new unsigned char[bufferLength];
	bool result = msg.SerializeToArray(buffer, bufferLength);
	if (true == result) {
		return buffer;
	}
	else {
		// TODO : error Ã³¸®
	}
}
size_t LoginProtocol::getSize()
{
	return msg.ByteSize();
}

void LoginProtocol::deSerialize(int* msgType,
	string& userId,
	string& passWord,
	int* authority,
	const unsigned char* serializedBuffer,
	const int serializedBufferSize)
{
	msg.ParseFromArray(serializedBuffer, serializedBufferSize);

	*msgType = msg.msg_type();
	userId = msg.user_id();
	passWord = msg.password();
	*authority = msg.authority();
}
