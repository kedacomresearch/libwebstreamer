#include <libwebstreamer/framework/pipeline.hpp>



namespace libwebstreamer
{
    namespace framework
    {

        Pipeline::Pipeline(const std::string &id)
            : id_(id)
            , bus_(NULL)
            , bus_watcher_(0)
        {
            pipeline_ = gst_pipeline_new(id.c_str());
            g_warn_if_fail(pipeline_);

            bus_ = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
            bus_watcher_ = gst_bus_add_watch(bus_, MessageHandler_, this);
            g_warn_if_fail(bus_);
            g_warn_if_fail(bus_watcher_);
        }

        gboolean Pipeline::MessageHandler_(GstBus *bus, GstMessage *message, gpointer data)
        {
            Pipeline *This = static_cast<Pipeline *>(data);
            return This->MessageHandler(message);
        }

        Pipeline::~Pipeline()
        {
            if (bus_ != NULL)
            {
                g_source_remove(bus_watcher_);
                gst_object_unref(bus_);
            }

            gst_element_set_state(pipeline_, GST_STATE_NULL);

            // for (int i = 0; i < endpoints.size(); i++)
            //     g_assert(endpoints[i].unique());
            //并发量高时，可能先收到delete pipeline的请求，再收到delete endpoint的请求
            //相对于remove_endpoint函数，这里少了on_remove_endpoint( * (*it) );这一步
            //但由于on_remove_endpoint( * (*it) );是虚函数，不能在析构函数中调用，所以需要分别在对应pipeline派生类中调用（例如在livestream的析构函数中调用）
            endpoints.erase(endpoints.begin(), endpoints.end());

            gst_object_unref(pipeline_);
        }

        bool Pipeline::add_endpoint(const std::shared_ptr<Endpoint> endpoint)
        {
            auto it = std::find_if(endpoints.begin(), endpoints.end(), [&endpoint](std::shared_ptr<Endpoint> &ep) {
                return endpoint->id() == ep->id();
            });
            if (it != endpoints.end())
            {
                return false;
            }
            endpoints.push_back(endpoint);
            return on_add_endpoint(endpoint);
        }

        bool Pipeline::remove_endpoint(const std::string &endpoint_id)
        {
            auto it = std::find_if(endpoints.begin(), endpoints.end(), [&endpoint_id](std::shared_ptr<Endpoint> &ep) {
                return endpoint_id == ep->id();
            });
            if (it == endpoints.end())
            {
                return false;
            }
            // g_assert(it->unique());
            on_remove_endpoint(*(*it));
            endpoints.erase(it);
            return true;
        }

        // void Pipeline::subscribe(const webstreamer::transport::webrtctopics::Endpoint &endpoint, int topics, const std::string &signalling_bridge, const std::string &notification_address)
        // {
        //     webstreamer::transport::webrtctopics::subscribe(endpoint, topics, signalling_bridge, notification_address, webstreamer::on_http_notification);
        // }
        // void Pipeline::unsubscribe(const webstreamer::transport::webrtctopics::Endpoint &endpoint, int topics, const std::string &signalling_bridge, const std::string &notification_address)
        // {
        //     webstreamer::transport::webrtctopics::unsubscribe(endpoint, topics, signalling_bridge, notification_address);
        // }
        // void Pipeline::push(const webstreamer::transport::webrtctopics::Endpoint &endpoint, const std::string &signalling_bridge, const std::map<webstreamer::transport::webrtctopics::Topic, std::string> &topics)
        // {
        //     webstreamer::transport::webrtctopics::push(endpoint, signalling_bridge, topics);
        // }
    }
}

// //------------------------------------------------------------
// #include <webstreamer/utility/pipejoint.hpp>
// #include "../gstutility/gst_rtsp_server.h"
// namespace webstreamer {
// 	namespace pipeline {
// 		static void on_rtsp_media_constructed(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data);

// 		void rtsp_server_new_rtsp_endpoint(const std::string& url, webstreamer::pipeline::Pipeline &pipeline)
// 		{
// 			GstRTSPServer *rtsp_server = gst_rtsp_server();
// 			GstRTSPMountPoints *mount_points = gst_rtsp_server_get_mount_points(rtsp_server);

// 			GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();
// 			gst_rtsp_media_factory_set_shared(factory, TRUE);
// 			gst_rtsp_media_factory_set_launch(factory, "( rtph264pay pt=96 name=pay0 )");

// 			g_signal_connect(factory, "media-constructed", (GCallback)on_rtsp_media_constructed, &pipeline);

// 			gst_rtsp_mount_points_add_factory(mount_points, url.c_str(), factory);
// 			g_object_unref(mount_points);
// 		}

// 		void on_rtsp_media_constructed(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data)
// 		{
// 			webstreamer::pipeline::Pipeline* pipeline = static_cast<webstreamer::pipeline::Pipeline*>(user_data);
// 			std::string media_type = "video";
// 			PipeJoint pipejoint = make_pipe_joint(media_type,"rtspserver_endpoint_joint");

// 			GstElement *rtsp_server_media_bin = gst_rtsp_media_get_element(media);
// 			g_warn_if_fail(gst_bin_add(GST_BIN(rtsp_server_media_bin), pipejoint.downstream_joint));

// 			GstElement *pay = gst_bin_get_by_name_recurse_up(GST_BIN(rtsp_server_media_bin), "pay0");
// 			g_warn_if_fail(gst_element_link(pipejoint.downstream_joint, pay));

// 			pipeline->add_pipe_joint(pipejoint.upstream_joint);
// 		}
// 	}
// }