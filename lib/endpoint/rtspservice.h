
#ifndef _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVICE_H_
#define _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVICE_H_


#include <framework/endpoint.h>
#include <framework/rtspserver.h>
#include <list>

class IRTSPService : public IEndpoint
{
public:
	IRTSPService( IApp* app, const std::string& name, RTSPServer::Type type);
	~IRTSPService();

	virtual bool Launch(const std::string& path, const std::string& launch, 
		         GCallback media_constructed, GCallback media_configure);
	virtual bool Stop();
protected:
	static void on_client_connected(GstRTSPServer *gstrtspserver, GstRTSPClient *client, gpointer user_data);
	static void on_tear_down(GstRTSPClient *client, GstRTSPContext *ctx, gpointer user_data);
private:
	GstRTSPMediaFactory * factory_;
	RTSPServer*           server_;
	std::string           path_;
	std::list<GstRTSPClient *> clients_;
};



#endif