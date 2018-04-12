
#ifndef _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVERVER_H___
#define _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVERVER_H___


#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

class RTSPServer
{
public:
	enum Type
	{
		RFC7826 =0,//RTSP 2.0 RFC7826
		ONVIF,
		UNDEFINED,
		SIZE= UNDEFINED,
		RTSP20 = RFC7826
	};
public:
	RTSPServer(Type type, int port =-1);
	~RTSPServer();
	bool Initialize(GstRTSPSessionPool* pool, int port);//TO BE DEL
	bool Initialize(GstRTSPSessionPool* pool, GMainContext* context=NULL);
	void Destroy();
	Type type() { return type_; }
	int port() { return port_; }
	GstRTSPServer* server() { return server_; }
protected:

	GstRTSPServer* server_;

	Type           type_;
	int            port_;
	

};



#endif