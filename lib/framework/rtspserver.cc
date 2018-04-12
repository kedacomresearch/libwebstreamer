#include "rtspserver.h"
#include <gst/rtsp-server/rtsp-onvif-server.h>
#include <webstreamer.h>
RTSPServer::RTSPServer(Type type ,int port)
	: server_(NULL)
	, type_(type)
	, port_(port)
{

}

RTSPServer::~RTSPServer()
{

}

bool RTSPServer::Initialize(GstRTSPSessionPool* pool, int port )
{
	//FIXME: onvif new link error
	//server_ = ( type_ == RFC7826 ? gst_rtsp_server_new() : gst_rtsp_onvif_server_new() );
	                                                         
	server_ = gst_rtsp_server_new() ;
	if (pool) {
		gst_rtsp_server_set_session_pool(server_, pool);
	}

	gchar* service = g_strdup_printf("%d", port);
	gst_rtsp_server_set_service(server_,service);
	g_free(service);

	//gst_rtsp_server_attach(server_, WebStreamer::main_context);

	return true;
}
bool RTSPServer::Initialize(GstRTSPSessionPool* pool, GMainContext* context /*= NULL*/)
{
	//FIXME: onvif new link error
	//server_ = ( type_ == RFC7826 ? gst_rtsp_server_new() : gst_rtsp_onvif_server_new() );

	server_ = gst_rtsp_server_new();
	if (pool) {
		gst_rtsp_server_set_session_pool(server_, pool);
	}

	gchar* service = g_strdup_printf("%d", port_);
	gst_rtsp_server_set_service(server_, service);
	g_free(service);

	gst_rtsp_server_attach(server_, context);

	return true;

}

void RTSPServer::Destroy()
{
	if (server_)
	{
		g_object_unref(server_);
		server_ = NULL;
	}
}