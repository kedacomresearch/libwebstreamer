#ifndef LIBWEBSTREAMER_APPLICATION_PIPELINE_LIVESTREAM_HPP
#define LIBWEBSTREAMER_APPLICATION_PIPELINE_LIVESTREAM_HPP

#include <framework/pipeline.hpp>

namespace libwebstreamer
{
    namespace application
    {
        namespace pipeline
        {

            class LiveStream : public libwebstreamer::framework::Pipeline
            {
            public:
                explicit LiveStream(const std::string &id);
                ~LiveStream();
                void add_pipe_joint(GstElement *upstream_joint);

                const std::string &rtsp_url() const
                {
                    return rtsp_url_;
                }
                std::string &rtsp_url()
                {
                    return rtsp_url_;
                }

            protected:
                virtual bool on_add_endpoint(const std::shared_ptr<libwebstreamer::framework::Endpoint> endpoint);
                virtual bool on_remove_endpoint(const std::shared_ptr<libwebstreamer::framework::Endpoint> endpoint);
                virtual bool MessageHandler(GstMessage *msg);

            private:
                GstElement *video_tee_;
                GstElement *audio_tee_;
                std::string rtsp_url_;
            };
        }
    }
}

#endif