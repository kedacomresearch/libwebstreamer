#ifndef __WEBSTREAMER_GST_RTSP_SERVER_H__
#define __WEBSTREAMER_GST_RTSP_SERVER_H__

#include <gst/rtsp-server/rtsp-server.h>

G_BEGIN_DECLS

GstRTSPServer *gst_rtsp_server();
void gst_rtsp_server_terminate();
void gst_rtsp_server_attach_(GMainContext *context);

G_END_DECLS

#endif