
#ifndef _LIBWEBSTREAMER_RTSPSERVER_H_
#define _LIBWEBSTREAMER_RTSPSERVER_H_

#include "processor.h"
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-onvif-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

#include "webstreamer.h"
class RTSPServer
{

public:

	RTSPServer(WebStreamer* ws,)
		: path_(path)
		, is_onvif_ (onvif)
		, webstreamer_(ws)

	{

	}

	bool Intialize(int port, onvif = false);
	void Destroy();

	WebStreamer* webstreamer() { return webstreamer_; }
	GstRTSPServer* server() { return server_; }
	bool IsOnvif(){
		return GST_IS_RTSP_ONVIF_SERVER(server_);
	}
protected:
	bool           is_onvif_;
	GstRTSPServer* server_;
	WebStreamer* webstreamer_;
};



class RTSPService
{

public:

	class RTSPService(RTSPServer* server )
		: server_(server)
		, factory_(NULL)
		, mount_(NULL)
	{

	}
	bool Initialize(const std::string& path, onvif = false);
	void Launch(const std::string& desc);
	bool IsOnvif() { return server_->IsOnvif; }
	
	WebStreamer* webstreamer() { return server_->webstreamer(); }
	RTSPServer*  server() { return server_; }
	GstRTSPMediaFactory* factory() { return factory_; }

protected:
	bool         is_onvif_;
	RTSPServer*  server_;
	GstRTSPMediaFactory* factory_;
	GstRTSPMountPoints*  mount_;
	
}

#endif