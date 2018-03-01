#ifndef LIBWEBSTREAMER_APPLICATION_ENDPOINT_RTSPCLIENT_HPP
#define LIBWEBSTREAMER_APPLICATION_ENDPOINT_RTSPCLIENT_HPP

#include <gst/gst.h>
#include <framework/Pipeline.hpp>
//#define ENABLE_AUDIO_CODEC
namespace libwebstreamer
{
    namespace application
    {
        namespace endpoint
        {

            class RtspClient : public libwebstreamer::framework::Endpoint
            {
            public:
                explicit RtspClient(const std::string &id, const std::string &type,
                                    const std::shared_ptr<libwebstreamer::framework::Pipeline> pipeline_owner);
                ~RtspClient()
                {
                }
                virtual void on_add_to_pipeline();
                virtual void on_remove_from_pipeline();

            private:
                void initialize(const std::string &url);
                static void on_rtspsrc_pad_added(GstElement *src, GstPad *src_pad, gpointer depay);

            private:
                GstElement *rtspsrc;
                GstElement *rtpdepay_video;
                GstElement *parse_video;
                GstElement *rtpaudiodepay;
#ifdef ENABLE_AUDIO_CODEC
                GstElement *alawdec;
#endif
            };
        }
    }
}

#endif