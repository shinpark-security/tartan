#include "MyProtocol.h"
#include <glib.h>

/*---------------------------
		CLoginProtocol
---------------------------*/
CLoginProtocol::CLoginProtocol()
:CBaseProtocol(MSG_LOGIN, &msg)
{	
}

CLoginProtocol::CLoginProtocol(const string id,const string pw)
:CBaseProtocol(MSG_LOGIN, &msg)
{
	setUserId(id);
	setUserPw(pw);
}
void CLoginProtocol::setUserId(const string id)
{
	msg.set_user_id(id);
}
void CLoginProtocol::setUserPw(const string password)
{
	msg.set_password(password);
}


/*---------------------------
		CImageProtocol
---------------------------*/
CImageProtocol::CImageProtocol(const int imgSize, const unsigned char* imgData)
:CBaseProtocol(MSG_IMAGE, &msg)
{
	setImage(imgSize,imgData);
}

void CImageProtocol::setImage(const int imgSize, const unsigned char* imgData)
{
	printf("CImageProtocol size=%d\n", imgSize);
	msg.set_img_size(imgSize);
	std::string data_str(reinterpret_cast<char const*>(imgData), imgSize);
	msg.set_img_data(data_str);
}


/*---------------------------
		CControlModeProtocol
---------------------------*/
CControlModeProtocol::CControlModeProtocol(protocol_msg::ControlMode::OpMode op)
:CBaseProtocol(MSG_CONTROL_MODE, &msg)
{
	msg.set_mode(op);
}


/*---------------------------
		CServerSettingProtocol
---------------------------*/
CServerSettingProtocol::CServerSettingProtocol(protocol_msg::ServerSetting::CtlMode ctl)
:CBaseProtocol(MSG_SERVER_SETTING, &msg)
{
	msg.set_mode(ctl);
}


/*---------------------------
		CVideoFileListProtocol
---------------------------*/
CVideoFileListProtocol::CVideoFileListProtocol(vector<string> &videos)
:CBaseProtocol(MSG_VIDEO_FILE_LIST, &msg)
{
	*msg.mutable_filelist() = { videos.begin(), videos.end() };	 
	msg.set_nfiles(videos.size());
}

/*---------------------------
		CTestMode_PlayVideoProtocol
---------------------------*/
CTestMode_PlayVideoProtocol::CTestMode_PlayVideoProtocol(uint32_t indx)
:CBaseProtocol(MSG_VIDEO_PLAY, &msg)
{
	msg.set_index(indx);
}

/*---------------------------
		CAckProtocol
---------------------------*/
CAckProtocol::CAckProtocol(protocol_msg::Ack::AckType type, uint32_t arg)
:CBaseProtocol(MSG_ACK, &msg)
{
	msg.set_acktype(type);
	msg.set_arg(arg);
}


/*---------------------------
		CLearningMode_AddUser
---------------------------*/
CLearningMode_AddUser::CLearningMode_AddUser(string name, uint32_t n_shots)
:CBaseProtocol(MSG_LEARNING_ADDUSER, &msg)
{
	msg.set_name(name);
	msg.set_n_shots(n_shots);
}

