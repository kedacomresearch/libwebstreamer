//#include "rtspserver"
//
//bool RTSPServer::Intialize(int port, bool onvif)
//{
//
//}
//
//bool RTSPService::Initialize(const std::string& path, bool onvif)
//{
//	mount_ = gst_rtsp_server_get_mount_points(this->server());
//	factory_ = gst_rtsp_media_factory_new();
//	/* if you want multiple clients to see the same video, set the shared property to TRUE */
//	gst_rtsp_media_factory_set_shared(factory_, TRUE);
//
//	gst_rtsp_media_factory_set_launch(factory, launch.c_str());
//	g_signal_connect(factory, "media-constructed", (GCallback)on_rtsp_media_constructed, (gpointer)(this));
//}
//
//void RTSPService::Launch(const std::string& desc)
//{
//	gst_rtsp_media_factory_set_launch(factory, desc.c_str());
//	gst_rtsp_mount_points_add_factory(mount_points, path_.c_str(), factory);
//	g_object_unref(mount_points);
//
//}