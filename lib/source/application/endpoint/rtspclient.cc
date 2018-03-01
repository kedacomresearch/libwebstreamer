#include <application/endpoint/rtspclinet.hpp>

namespace libwebstreamer
{
    namespace application
    {
        namespace endpoint
        {
            namespace mediaservice = ::webstreamer::transport::mediaservice;

            RtspClient::RtspClient(const std::string &id, const std::string &type,
                                   const std::shared_ptr<libwebstreamer::framework::Pipeline> pipeline_owner)
                : libwebstreamer::framework::Endpoint(id, type, pipeline_owner)
            {
                // initialize(ep);
            }

            void RtspClient::initialize(const std::string &url)
            {
                rtspsrc = gst_element_factory_make("rtspsrc", "rtspsrc");

                configuration::PortRange port_range;
                webstreamer::configuration::query_rtspsrc_port_range(port_range);
                g_object_set(G_OBJECT(rtspsrc), "location", url.c_str(),
                             "port-range", webstreamer::configuration::to_string(port_range), NULL);
            }

            void RtspClient::on_rtspsrc_pad_added(GstElement *src, GstPad *src_pad, gpointer rtspclient)
            {
                RtspClient *rtsp_client = static_cast<RtspClient *>(rtspclient);
                GstCaps *caps = gst_pad_query_caps(src_pad, NULL);
                GstStructure *stru = gst_caps_get_structure(caps, 0);
                const GValue *media_type = gst_structure_get_value(stru, "media");

                if (g_str_equal(g_value_get_string(media_type), "video"))
                {
                    GstPad *sink_pad = gst_element_get_static_pad(GST_ELEMENT_CAST(rtsp_client->rtpdepay_video), "sink");
                    GstPadLinkReturn ret = gst_pad_link(src_pad, sink_pad);
                    g_warn_if_fail(ret == GST_PAD_LINK_OK);
                    gst_object_unref(sink_pad);
                }
                else if (g_str_equal(g_value_get_string(media_type), "audio"))
                {
                    GstPad *sink_pad = gst_element_get_static_pad(GST_ELEMENT_CAST(rtsp_client->rtpaudiodepay), "sink");
                    GstPadLinkReturn ret = gst_pad_link(src_pad, sink_pad);
                    g_warn_if_fail(ret == GST_PAD_LINK_OK);
                    gst_object_unref(sink_pad);
                }
                else
                {
                    g_warn_if_reached();
                }
            }

            void RtspClient::on_add_to_pipeline()
            {
                switch (pipeline_owner->video_encoding_)
                {
                    case mediaservice::VideoEncodingType::VIDEO_TYPE_H264:
                        rtpdepay_video = gst_element_factory_make("rtph264depay", "depay");
                        parse_video = gst_element_factory_make("h264parse", "parse");
                        break;
                    case mediaservice::VideoEncodingType::VIDEO_TYPE_H265:
                        rtpdepay_video = gst_element_factory_make("rtph265depay", "depay");
                        parse_video = gst_element_factory_make("h265parse", "parse");
                        break;
                    default:
                        g_warn_if_reached();
                        break;
                }
                g_warn_if_fail(rtpdepay_video && parse_video);

                gst_bin_add_many(GST_BIN(pipeline_owner().pipeline()), rtspsrc, rtpdepay_video, parse_video, NULL);
                g_signal_connect(rtspsrc, "pad-added", (GCallback)on_rtspsrc_pad_added, this);
                g_warn_if_fail(gst_element_link(rtpdepay_video, parse_video));

                switch (pipeline_owner->audio_encoding_)
                {
                    case mediaservice::AudioEncodingType::AUDIO_TYPE_PCMA:
                        rtpaudiodepay = gst_element_factory_make("rtppcmadepay", "audio-depay");
                        break;
                    case mediaservice::AudioEncodingType::AUDIO_TYPE_PCMU:
                        rtpaudiodepay = gst_element_factory_make("rtppcmudepay", "audio-depay");
                        break;
                    case mediaservice::VideoEncodingType::VIDEO_TYPE_H265:
                        rtpaudiodepay = gst_element_factory_make("rtpopusdepay", "audio-depay");
                        break;
                    default:
                        g_message("No Audio!");
                        break;
                }

#ifdef ENABLE_AUDIO_CODEC
                alawdec = gst_element_factory_make("alawdec", "alawdec");
                g_warn_if_fail(rtpaudiodepay && alawdec);
                gst_bin_add_many(GST_BIN(pipeline_owner().pipeline()), rtpaudiodepay, alawdec, NULL);
                g_warn_if_fail(gst_element_link(rtpaudiodepay, alawdec));
#else
                g_warn_if_fail(rtpaudiodepay);
                gst_bin_add_many(GST_BIN(pipeline_owner().pipeline()), rtpaudiodepay, NULL);
#endif
            }

            void RtspClient::on_remove_from_pipeline()
            {
                gst_bin_remove_many(GST_BIN(pipeline_owner().pipeline()), rtspsrc, rtpdepay_video, parse_video, NULL);
                gst_bin_remove_many(GST_BIN(pipeline_owner().pipeline()), rtpaudiodepay, NULL);
            }
        }
    }
}