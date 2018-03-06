#include <framework/pipeline.hpp>

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
            bus_watcher_ = gst_bus_add_watch(bus_, message_handler, this);
            g_warn_if_fail(bus_);
            g_warn_if_fail(bus_watcher_);
        }

        gboolean Pipeline::message_handler(GstBus *bus, GstMessage *message, gpointer data)
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
            g_assert(endpoints_.empty());

            // for (int i = 0; i < endpoints_.size(); i++)
            // {
            //     g_assert(endpoints_[i].unique());
            //     // on_remove_endpoint(endpoints_[i]);
            // }
            // endpoints_.erase(endpoints_.begin(), endpoints_.end());

            gst_object_unref(pipeline_);
        }

        bool Pipeline::add_endpoint(const std::shared_ptr<Endpoint> endpoint)
        {
            auto it = std::find_if(endpoints_.begin(), endpoints_.end(), [&endpoint](std::shared_ptr<Endpoint> &ep) {
                return endpoint->id() == ep->id();
            });
            if (it != endpoints_.end())
            {
                return false;
            }
            if (!on_add_endpoint(endpoint))
            {
                return false;
            }
            endpoints_.push_back(endpoint);
            return true;
        }

        bool Pipeline::remove_endpoint(const std::string &endpoint_id)
        {
            auto it = std::find_if(endpoints_.begin(), endpoints_.end(), [&endpoint_id](std::shared_ptr<Endpoint> &ep) {
                return endpoint_id == ep->id();
            });
            if (it == endpoints_.end())
            {
                return false;
            }
            g_assert(it->unique());
            on_remove_endpoint(*it);
            endpoints_.erase(it);
            printf("-------endpoints left: %d----\n", endpoints_.size());
            return true;
        }
        bool Pipeline::remove_all_endpoints()
        {
            for (int i = 0; i < endpoints_.size(); i++)
            {
                g_assert(endpoints_[i].unique());
                on_remove_endpoint(endpoints_[i]);
            }
            endpoints_.erase(endpoints_.begin(), endpoints_.end());
            printf("-------endpoints left: %d----\n", endpoints_.size());
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