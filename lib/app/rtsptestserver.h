
#ifndef _LIBWEBSTREAMER_RTSP_TEST_SERVER_H_
#define _LIBWEBSTREAMER_RTSP_TEST_SERVER_H_

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

#include <framework/app.h>



class RTSPTestServer : public IApp
{
public:
	APP(RTSPTestServer)


	RTSPTestServer(const std::string& name, WebStreamer* ws )
		: IApp( name ,ws)
		//, server_(NULL)
		,mounts_(NULL)
		,factory_(NULL)
		//,session_(NULL)
		, rtspservice_(NULL)
	{
		
	}

	void On(Promise* promise);
	bool Initialize(Promise* promise);
	bool Destroy(Promise* promise);
protected:
	void Startup(Promise* promise);
	void Stop(Promise* promise);

private:
	//GstRTSPServer*       server_;
	GstRTSPMountPoints*  mounts_;
	GstRTSPMediaFactory* factory_;
	//GstRTSPSessionPool * session_;
	IEndpoint*           rtspservice_;
};


#endif //!_LIBWEBSTREAMER_RTSP_TEST_SERVER_H_