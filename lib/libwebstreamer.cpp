#include <owr/owr.h>
#include <json-glib/json-glib.h>
#include "libwebstreamer.hpp"
#include <stdio.h>
LIBWEBSTREAMER_NS_BEGIN

	static GMainContext *libwebstreamer_main_context = NULL;

	void initialize(const char* json, size_t size)
	{
		if (json)
		{
			//do parse 
			printf("init param <%s>\n", json);
		}
		printf("gstreamer init!\n");
		libwebstreamer_main_context = g_main_context_ref_thread_default();
		owr_init(libwebstreamer_main_context);
		owr_run_in_background();
		printf("gstreamer init done.\n");

	}

	void terminate()
	{
		owr_quit();
		printf("gstreamer quite.\n");
	}

	void schedule_with_user_data(GSourceFunc callback, gpointer user_data)
	{
		GSource *source = g_idle_source_new();

		g_source_set_callback(source, callback, user_data, NULL);
		g_source_set_priority(source, G_PRIORITY_DEFAULT);
		g_source_attach(source, libwebstreamer_main_context);
	}



LIBWEBSTREAMER_NS_END