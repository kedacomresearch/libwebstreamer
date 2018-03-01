#include <application/pipeline/livestream.hpp>
#include <application/endpoint/rtspclient.hpp>
#include <application/endpoint/rtspserver.hpp>
#include <application/endpoint/webrtc.hpp>

namespace libwebstreamer
{
    namespace application
    {
        namespace pipeline
        {
            using namespace libwebstreamer::framework;

            LiveStream::LiveStream(const std::string &id)
                : Pipeline(id)
            {
                video_tee = gst_element_factory_make("tee", "video-tee");
                audio_tee = gst_element_factory_make("tee", "audio-tee");
                g_warn_if_fail(gst_bin_add(GST_BIN(pipeline()), video_tee));
                g_warn_if_fail(gst_bin_add(GST_BIN(pipeline()), audio_tee));
            }

            //debug
            LiveStream::~LiveStream()
            {
                gst_element_set_state(pipeline_, GST_STATE_NULL);
                for (auto it : endpoints)
                {
                    on_remove_endpoint(*it);
                }
            }

            void LiveStream::add_pipe_joint(GstElement *upstream_joint)
            {
                g_warn_if_fail(gst_bin_add(GST_BIN(pipeline()), upstream_joint));

                gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(upstream_joint), "media-type");
                if (g_str_equal(media_type, "video"))
                {
                    g_warn_if_fail(gst_element_link(video_tee, upstream_joint));
                }
                else if (g_str_equal(media_type, "audio"))
                {
                    g_warn_if_fail(gst_element_link(audio_tee, upstream_joint));
                }
                GstStateChangeReturn ret = gst_element_set_state(Pipeline::pipeline(), GST_STATE_PLAYING);
            }

            bool LiveStream::on_add_endpoint(const std::shared_ptr<Endpoint> endpoint)
            {
                switch (endpoint->type())
                {
                    case EndpointType::RTSP:
                    {
                        // static_cast<libwebstreamer::application::endpoint::RtspClient *>(&endpoint)->add_to_pipeline();
                        endpoint->add_to_pipeline();
                        GstElement *parse = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline()), "parse");
                        g_warn_if_fail(parse);
                        g_warn_if_fail(gst_element_link(parse, video_tee));
#ifdef ENABLE_AUDIO_CODEC
                        GstElement *alawdec = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline()), "alawdec");
                        g_warn_if_fail(alawdec);
                        g_warn_if_fail(gst_element_link(alawdec, audio_tee));
#else
                        GstElement *rtppcmadepay = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline()), "audio-depay");
                        g_warn_if_fail(rtppcmadepay);
                        g_warn_if_fail(gst_element_link(rtppcmadepay, audio_tee));
#endif
                    }
                    break;
                    // case webstreamer::transport::mediaservice::EndpointType::MEDIA_ENDPOINT_RTSPSERVER:
                    //     static_cast<webstreamer::pipeline::endpoint::RtspServer *>(&endpoint)->add_to_pipeline(*this);
                    //     break;
                    case EndpointType::WEBRTC:
                        // static_cast<webstreamer::pipeline::endpoint::WebRTC *>(&endpoint)->add_to_pipeline(*this);
                        // static_cast<webstreamer::pipeline::endpoint::WebRTC *>(&endpoint)->disable_signalling_incoming_feedback();
                        break;
                    default:
                        break;
                }

                return true;
            }

            bool LiveStream::on_remove_endpoint(const std::shared_ptr<Endpoint> endpoint)
            {
                // switch (endpoint.type())
                // {
                //     case webstreamer::transport::mediaservice::EndpointType::MEDIA_ENDPOINT_RTSPCLIENT:
                //         static_cast<webstreamer::pipeline::endpoint::RtspClient *>(&endpoint)->remove_from_pipeline(*this);
                //         break;
                //     case webstreamer::transport::mediaservice::EndpointType::MEDIA_ENDPOINT_RTSPSERVER:
                //         static_cast<webstreamer::pipeline::endpoint::RtspServer *>(&endpoint)->remove_from_pipeline(*this);
                //         break;
                //     case webstreamer::transport::mediaservice::EndpointType::MEDIA_ENDPOINT_WEBRTC:
                //         static_cast<webstreamer::pipeline::endpoint::WebRTC *>(&endpoint)->remove_from_pipeline(*this);
                //         break;
                //     default:
                //         return false;
                // }
                return true;
            }
            bool LiveStream::MessageHandler(GstMessage *msg)
            {
                if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
                {
                    GError *err = NULL;
                    gchar *dbg_info = NULL;

                    gst_message_parse_error(msg, &err, &dbg_info);
                    g_critical("errors occured in pipeline: livestream!\n---------------------------\nERROR from element %s: %s\nDebugging info: %s\n---------------------------\n",
                               GST_OBJECT_NAME(msg->src), err->message, (dbg_info) ? dbg_info : "none");

                    g_error_free(err);
                    g_free(dbg_info);
                }
                if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_WARNING)
                {
                    GError *warn = NULL;
                    gchar *dbg_info = NULL;

                    gst_message_parse_warning(msg, &warn, &dbg_info);
                    g_warning("warnning occured in pipeline: livestream!\n---------------------------\nERROR from element %s: %s\nDebugging info: %s\n---------------------------\n",
                              GST_OBJECT_NAME(msg->src), warn->message, (dbg_info) ? dbg_info : "none");

                    g_error_free(warn);
                    g_free(dbg_info);
                }
                if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_INFO)
                {
                    GError *warn = NULL;
                    gchar *dbg_info = NULL;

                    gst_message_parse_info(msg, &warn, &dbg_info);
                    g_message("info message in pipeline: livestream!\n---------------------------\nERROR from element %s: %s\nDebugging info: %s\n---------------------------\n",
                              GST_OBJECT_NAME(msg->src), warn->message, (dbg_info) ? dbg_info : "none");

                    g_error_free(warn);
                    g_free(dbg_info);
                }
                return true;
            }
        }
    }
}