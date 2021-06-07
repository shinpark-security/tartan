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

#include "main.h"
#include "imgproc.h"
#include "comm.h"


gboolean thread_start(tServiceData *psbd);
gboolean thread_stop(tServiceData *psbd);
static gboolean on_handle_sigterm(gpointer pUserData);



static gboolean on_handle_sigterm(gpointer pUserData)
{
  tServiceData *psbd=(tServiceData *)pUserData;
  gboolean ret=FALSE;
  printf("SIGTERM Received...\n");

  if (psbd != nullptr)  {
      /* quit main loop */
      if ( psbd->mainloop != nullptr ) {
        thread_stop(psbd);
        g_main_loop_quit(psbd->mainloop);
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
    //     psbd->queue = g_async_queue_new ();
      psbd->thread_run=true;
      psbd->thread = g_thread_new ("service_main", service_main, psbd);
      if (psbd->thread != nullptr) {
          ret=true;
      }
    }
    return ret;
}

gpointer service_main (gpointer data) {
  tServiceData *psbd=(tServiceData *)data;

  CComm *pcom=new CComm();
  CImgProc *pimg=new CImgProc();

  pcom->start(psbd->tcp_port);
  pimg->start();

  while(psbd->thread_run) {

    sleep(1);
    printf("main thread...\n");
  }


  delete pcom;
  pcom = nullptr;

  delete pimg;
  pimg = nullptr;

}


gboolean thread_stop(tServiceData *psbd)
{
    gboolean ret=false;
    printf("thread_stop()+\n");

    if (psbd != nullptr) {
        // g_async_queue_push (ppowm->queue, ((gpointer) (glong*) (glong)(-1)) );
      psbd->thread_run=false;
      (void)g_thread_join (psbd->thread);
      // g_async_queue_unref (ppowm->queue);
      ret=true;
    }
    return ret;
}

int main(int argc, char *argv[])
{
  tServiceData sbd;
  sbd.mainloop = g_main_loop_new(nullptr, false);
  if (sbd.mainloop !=nullptr) {
      guint sig1,sig2;

    sbd.tcp_port=2000;
    // (void)configureWdogTimer(ppowm);

    if (thread_start(&sbd)) {
        sig1=g_unix_signal_add(SIGINT, &on_handle_sigterm, nullptr);
        sig2=g_unix_signal_add(SIGTERM, &on_handle_sigterm, nullptr);
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

  return 0;
}

