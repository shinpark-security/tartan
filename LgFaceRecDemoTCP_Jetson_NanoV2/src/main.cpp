#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdarg.h>
// #include <gio/gio.h>                        /* GLib lists                     */
#include <syslog.h> /* Syslog messages                */
// #include <systemd/sd-daemon.h>              /* Systemd wdog                   */
#include <glib-unix.h>
#include <glib.h>
#include <unistd.h>
#include "common.h"

#include "mymsg.h"
#include "main.h"
#include "imgproc.h"
#include "comm.h"
#include "mydb.h"
#include "cyper.h"
#include "ProtocolManager.h"
#include "BaseProtocol.h"
#include "MyProtocol.h"

/* prototypes */
gboolean steate_machine(tServiceData *psbd);
gboolean thread_start(tServiceData *psbd);
gboolean thread_stop(tServiceData *psbd);
static gboolean on_handle_sigterm(gpointer pUserData);
static gpointer main_thread(gpointer data);


/* implementation */
static gboolean on_handle_sigterm(gpointer pUserData)
{
	tServiceData *psbd = (tServiceData *)pUserData;
	gboolean ret = FALSE;
	printf("SIGTERM Received...\n");

	if (psbd != nullptr)
	{
		/* quit main loop */
		if (psbd->mainloop != nullptr)
		{
			g_main_loop_quit(psbd->mainloop);
			thread_stop(psbd);
		}
		ret = TRUE;
	}
	return ret;
}

gboolean thread_start(tServiceData *psbd)
{
	gboolean ret = true;
	printf("thread_start()+\n");
	if (psbd != nullptr)
	{
		//     psbd->pworker_msg_handler = worker_msg_handler;
		psbd->thread_run = true;
		psbd->main_thread = g_thread_new("main_thread", main_thread, psbd);
		if (psbd->main_thread != nullptr)
		{
			ret = true;
		}
	}
	return ret;
}

gboolean steate_machine(tServiceData *psbd)
{
	static SystemState prev_sstate=SS_READY;
	if (psbd->sstate != prev_sstate ) {
		printf("SYSTEM STATE: %d\n",psbd->sstate );
	}
	switch(psbd->sstate)
	{
	case SS_READY:
	break;
	case SS_LOGIN:
	break;
	case SS_LOGIN_OK:
	break;
	case SS_LOGIN_NOK:
	break;
	case SS_RUN:
	break;
	case SS_LEARN_START:
	break;
	case SS_LEARN:
	break;
	case SS_LEARN_DONE:
	break;
	case SS_TESTRUN_START:
	break;
	break;
	case SS_TESTRUN:
	break;
	case SS_TESTRUN_DONE:
	break;
	}
	prev_sstate=psbd->sstate;
	return true;
}


gpointer main_thread(gpointer data)
{
	if (data == nullptr)
		return nullptr;
	tServiceData *psbd = (tServiceData *)data;

	psbd->pcom->start();
	psbd->pcom_tls->start();
	psbd->pimgproc->start();

	while (psbd->thread_run)
	{
		MyMsg *pmsg = (MyMsg *)g_async_queue_timeout_pop(psbd->queue, 1000000U);
		if (pmsg == GINT_TO_POINTER(-1))
		{
			printf("Exitting.....\n");
			break;
		}
		if (pmsg)
		{
			switch (pmsg->msgid)
			{
			case MYMSG_FRAME:
			{
				printf(".");
				if (psbd->pcom->thread_run)
					psbd->pcom->send_jpg(pmsg->mat);
				if (psbd->pcom_tls->thread_run)
					psbd->pcom_tls->send_jpg(pmsg->mat);
				pmsg->mat.release();
			}
			break;
			case MYMSG_FROM_CLIENT:
			{
				// printf("MSG:%d\n",pmsg->msgid);
				// char *cmd = (char *)pmsg->pdata;
				CComm *pcom = (CComm *)pmsg->pobj;
				CBaseProtocol *pbase=(CBaseProtocol *)pmsg->pdata;
				// printf("CONTROL MESSAGE RECEIVED: %10s\n", cmd);
				// psbd->pcom->send_response((const unsigned char *)"response",5);
				// psbd->pimgproc->add_new_user("dolmangi");
				printf("MYMSG_FROM_CLIENT msg_type=%d\n", pbase->msg_type);
				if (pbase->msg_type == MSG_LOGIN)
				{
					CLoginProtocol *plogin=dynamic_cast<CLoginProtocol*>(pbase);
					string account=plogin->msg.user_id();
					string passwd=plogin->msg.password();
					printf("ACCOUNT=%s PASSWORD=%s\n ", account.c_str(), passwd.c_str());
					CAuth auth;
					int priv = auth.login(account, passwd);
					if (priv == 0)
					{
						printf("OK You're an administrator\n");
						psbd->pimgproc->set_enable_send(true);
					}
					else if (priv > 0)
					{
						printf("OK You're a normal user\n");
						psbd->pimgproc->set_enable_send(true);
					}
					else
					{
						printf("Login fail.\n");
						psbd->pimgproc->set_enable_send(false);
						if (pcom)
							pcom->disconnect();
					}
				}
				else if (pbase->msg_type == MSG_SERVER_SETTING)
				{
					CServerSettingProtocol *ctl=dynamic_cast<CServerSettingProtocol*>(pbase);

					printf("Server Setting MODE=%d\n ", ctl->msg.mode());

					if (ctl->msg.mode() == protocol_msg::ServerSetting::CAM_STOP) {
						psbd->pimgproc->set_enable_send(false);
					}
					else if (ctl->msg.mode() == protocol_msg::ServerSetting::CAM_START) {
						psbd->pimgproc->set_enable_send(true);
					}
				}
				else if (pbase->msg_type == MSG_CONTROL_MODE)
				{
					CControlModeProtocol *ctl=dynamic_cast<CControlModeProtocol*>(pbase);

					printf("Control MODE=%d\n ", ctl->msg.mode());

					if (ctl->msg.mode() == protocol_msg::ControlMode::RUN) {
						psbd->pimgproc->set_enable_send(false);
						psbd->pimgproc->stop();
					}
					else if (ctl->msg.mode() == protocol_msg::ControlMode::LEARNING) {
						psbd->pimgproc->video_file="../friends640x480.mp4";
						psbd->pimgproc->start(IMGPROC_MODE_TESTRUN);
						psbd->pimgproc->set_enable_send(true);
					}
					else if (ctl->msg.mode() == protocol_msg::ControlMode::TESTRUN) {
						psbd->pimgproc->start(IMGPROC_MODE_RUN);
						psbd->pimgproc->set_enable_send(true);
					}
				}
				delete pbase;
			}
			break;
			case MYMSG_NET_CONNECTED:
			{
				printf("MYMSG_NET_CONNECTED\n");
				// printf("pcom=%p  pobj=%p\n",psbd->pcom, pmsg->pobj);
				// if (psbd->pcom==pmsg->pobj)
				// 	psbd->pcom_tls->pause();
				// else
				// 	psbd->pcom->pause();
			}
			break;
			case MYMSG_NET_DISCONNECTED:
			{
				printf("MYMSG_NET_DISCONNECTED\n");
				// if (psbd->pcom==pmsg->pobj)
				// 	psbd->pcom_tls->resume();
				// else
				// 	psbd->pcom->resume();
			}
			break;
			}
			delete pmsg;
		}
		steate_machine(psbd);
		// printf("main thread...\n");
#if 1 //temp for test
		// psbd->pimgproc->set_enable_send(true);
#endif
	}

	if (psbd->pimgproc) {
		psbd->pimgproc->stop();
	}
	if (psbd->pcom)
	{
		psbd->pcom->stop();
	}
	if (psbd->pcom_tls) {
		psbd->pcom_tls->stop();
	}
}

gboolean thread_stop(tServiceData *psbd)
{
	gboolean ret = false;
	printf("thread_stop()+\n");

	if (psbd != nullptr)
	{
		g_async_queue_push(psbd->queue, ((gpointer)(glong *)(glong)(-1)));
		if (psbd->pcom)
		{
			psbd->pcom->stop();
		}
		if (psbd->pcom_tls)
		{
			psbd->pcom_tls->stop();
		}
		if (psbd->pimgproc)
		{
			psbd->pimgproc->stop();
		}
		psbd->thread_run = false;
		(void)g_thread_join(psbd->main_thread);
		ret = true;
	}
	return ret;
}

extern void print_pkt_header(const unsigned char* buff,int size) ;

int main(int argc, char *argv[])
{
	// // CMydb db;
	// // // db.start();
	// // // db.initialize_database_faces();
	// // db.list_faces();
	// // // db.list_alluser();

	// // return 0;
	// // // CAuth auth;
	// CCyper cyp;
	// unsigned char a[50];
	// unsigned char b[50];
	// memset(a,0,50);
	// memset(b,0,50);
	// sprintf((char*)&a[0],"1234567890123456");
	// cyp.encrypt_aes(a,strlen((char*)a), b);
	// print_pkt_header(b,50);
	// printf("len a=%zu\n",strlen((char*)a));
	// printf("len b=%zu\n",strlen((char*)b));

	// int len=strlen((char*)a)/16 * 16 + (strlen((char*)a) % 16 ? 16 :0 ); 
	// printf("original len=%d  cal len=%d\n", strlen((char*)a), len );
	// memset(a,0,50);
	// cyp.decrypt_aes(b, 32, a);
	// print_pkt_header(a,50);
	// printf("a=%s\n", a);

	// return 0;


	// // int ret=db.check_passwd("lg",auth.get_passwd_enc("lg1234"));
	// // printf("RET=%d\n", ret);
	// // // auth.get_passwd_enc("lg1234");
	tServiceData sbd;
	sbd.sstate=SS_READY;
	sbd.queue = g_async_queue_new();
	sbd.pcom = new CComm(false, TCP_PORT_NON_SECURE, sbd.queue);
	sbd.pcom_tls = new CComm(true, TCP_PORT_SECURE, sbd.queue);
	if (argc == 2)
	{
		sbd.pimgproc = new CImgProc(sbd.queue , argv[1]);
	}
	else
		sbd.pimgproc = new CImgProc(sbd.queue );

    /* Initialize wolfSSL */
    wolfSSL_Init();

	sbd.mainloop = g_main_loop_new(nullptr, false);
	if (sbd.mainloop != nullptr)
	{
		guint sig1, sig2;
		// (void)configureWdogTimer(psbd);

		if (thread_start(&sbd))
		{
			sig1 = g_unix_signal_add(SIGINT, &on_handle_sigterm, &sbd);
			sig2 = g_unix_signal_add(SIGTERM, &on_handle_sigterm, &sbd);
			printf("starting main loop....\n");

			//     (void)sd_notify(0, "READY=1");
			g_main_loop_run(sbd.mainloop);

			printf("main loop exit...\n");

			(void)g_source_remove(sig1);
			(void)g_source_remove(sig2);
		}

		g_main_loop_unref(sbd.mainloop);
		sbd.mainloop = nullptr;
		g_async_queue_unref(sbd.queue);
	}

	// for(iter=sbd.list_com.begin(); iter!=sbd.list_com.end(); iter++)
	if (sbd.pcom)
	{
		delete sbd.pcom;
		sbd.pcom = nullptr;
	}
	if (sbd.pcom_tls)
	{
		delete sbd.pcom_tls;
		sbd.pcom_tls = nullptr;
	}
	if (sbd.pimgproc)
	{
		delete sbd.pimgproc;
		sbd.pimgproc = nullptr;
	}
	
	wolfSSL_Cleanup();
	return 0;
}
