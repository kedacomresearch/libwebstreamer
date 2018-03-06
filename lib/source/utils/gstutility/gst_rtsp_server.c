#include "gst_rtsp_server.h"

static GstRTSPServer *rtsp_server_ = NULL;
static guint rtsp_server_source_id = 0;

GstRTSPServer *gst_rtsp_server()
{
	if (rtsp_server_ == NULL)
	{
		rtsp_server_ = gst_rtsp_server_new();
		g_warn_if_fail(rtsp_server_);
		g_return_val_if_fail(rtsp_server_, NULL);
	}
	return rtsp_server_;
}

void gst_rtsp_server_terminate()
{
	g_return_if_fail(rtsp_server_);
	g_return_if_fail(rtsp_server_source_id);

	g_warn_if_fail(g_source_remove(rtsp_server_source_id));
	g_object_unref(rtsp_server_);
}

void gst_rtsp_server_attach_(GMainContext *context)
{
	g_return_if_fail(rtsp_server_);
	g_return_if_fail(context);

	rtsp_server_source_id = gst_rtsp_server_attach(rtsp_server_, context);
}