#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdarg.h>
// #include <gio/gio.h>                        /* GLib lists                     */
#include <syslog.h>                         /* Syslog messages                */
// #include <systemd/sd-daemon.h>              /* Systemd wdog                   */
#include <glib-unix.h>
#include <glib.h>
#include <unistd.h>
#include "common.h"

#include "mymsg.h"
#include "main.h"
#include "imgproc.h"
#include "comm.h"


gboolean thread_start(tServiceData *psbd);
gboolean thread_stop(tServiceData *psbd);
static gboolean on_handle_sigterm(gpointer pUserData);
static gpointer main_thread (gpointer data);


static gboolean on_handle_sigterm(gpointer pUserData)
{
	tServiceData *psbd=(tServiceData *)pUserData;
	gboolean ret=FALSE;
	printf("SIGTERM Received...\n");

	if (psbd != nullptr)  {
		/* quit main loop */
		if ( psbd->mainloop != nullptr ) {
			g_main_loop_quit(psbd->mainloop);
			thread_stop(psbd);
		}
		ret=TRUE;
	}
	return ret;
}



gboolean thread_start(tServiceData *psbd)
{
	gboolean ret=true;
	printf("thread_start()+\n");
	if (psbd != nullptr) {
		//     psbd->pworker_msg_handler = worker_msg_handler;
		psbd->queue = g_async_queue_new ();
		psbd->thread_run=true;
		psbd->main_thread = g_thread_new ("main_thread", main_thread, psbd);
		if (psbd->main_thread != nullptr) {
			ret=true;
		}
	}
	return ret;
}

gpointer main_thread (gpointer data) {
	if (data==nullptr) return nullptr;
	tServiceData *psbd=(tServiceData *)data;

	psbd->pcom->start(psbd->tcp_port,psbd->queue);
	psbd->pimgproc->start(psbd->queue);

	while(psbd->thread_run) {
		MyMsg* pmsg = (MyMsg*) g_async_queue_timeout_pop  (psbd->queue, 1000000U);
		if (pmsg == GINT_TO_POINTER (-1))
		{
			printf("Exitting.....\n");
			break;
		}
		if (pmsg) {
			switch(pmsg->msgid) {
				case MYMSG_FRAME: {
									  printf(".");
									  psbd->pcom->send_jpg(pmsg->mat);
									  pmsg->mat.release();
								  }
								  break;
				case MYMSG_CONTROL: {
										// printf("MSG:%d\n",pmsg->msgid);
										char* cmd=(char*)pmsg->pdata;
										printf("CONTROL MESSAGE RECEIVED: %10s\n",cmd);
										// psbd->pcom->send_response((const unsigned char *)"response",5);
										// psbd->pimgproc->add_new_user("dolmangi");
										delete cmd;
									}
									break;
			}
			delete pmsg;
		}
		psbd->pimgproc->set_enable_send(psbd->pcom->tcp_connected);
		// printf("main thread...\n");
	}

	if (psbd->pcom) {
		psbd->pcom->stop();
	}
	if (psbd->pimgproc) {
		psbd->pimgproc->stop();
	}

}


gboolean thread_stop(tServiceData *psbd)
{
	gboolean ret=false;
	printf("thread_stop()+\n");

	if (psbd != nullptr) {
		g_async_queue_push (psbd->queue, ((gpointer) (glong*) (glong)(-1)) );
		if (psbd->pcom) {
			psbd->pcom->stop();
		}
		if (psbd->pimgproc) { 
			psbd->pimgproc->stop();
		}
		psbd->thread_run=false;
		(void)g_thread_join (psbd->main_thread);
		g_async_queue_unref (psbd->queue);
		ret=true;
	}
	return ret;
}

int main(int argc, char *argv[])
{
	tServiceData sbd;
	sbd.pcom=new CComm();
	if (argc==2) {
		sbd.pimgproc=new CImgProc(argv[1]);
	}
	else
		sbd.pimgproc=new CImgProc();

	sbd.mainloop = g_main_loop_new(nullptr, false);
	if (sbd.mainloop !=nullptr) {
		guint sig1,sig2;

		sbd.tcp_port=2000;
		// (void)configureWdogTimer(psbd);

		if (thread_start(&sbd)) {
			sig1=g_unix_signal_add(SIGINT, &on_handle_sigterm, &sbd);
			sig2=g_unix_signal_add(SIGTERM, &on_handle_sigterm, &sbd);
			printf("starting main loop....\n");

			//     (void)sd_notify(0, "READY=1");
			g_main_loop_run(sbd.mainloop);

			printf("main loop exit...\n");

			(void)g_source_remove(sig1);
			(void)g_source_remove(sig2);
		}

		g_main_loop_unref(sbd.mainloop);
		sbd.mainloop = nullptr;

	}

	if (sbd.pcom) {
		delete sbd.pcom;
		sbd.pcom = nullptr;
	}

	if (sbd.pimgproc) {
		delete sbd.pimgproc;
		sbd.pimgproc = nullptr;
	}
	return 0;
}
