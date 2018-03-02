#include <fbs/webstreamer_generated.h>
#include <framework/pipeline_manager.hpp>
#include <application/pipeline/livestream.hpp>

namespace libwebstreamer
{
    namespace framework
    {
        using namespace libwebstreamer::util;
        using namespace libwebstreamer::application::pipeline;

        void PipelineManager::call(const void *data, size_t size, const callback_fn &cb)
        {
            ::flatbuffers::Verifier verifier((const uint8_t *)data, size);
            if (webstreamer::VerifyrootBuffer(verifier))
            {
                auto livestream_any = webstreamer::Getroot((const uint8_t *)data);
                switch (livestream_any->any_type())
                {
                    case webstreamer::Any_webstreamer_livestreamer_Create:
                    {
                        auto create = livestream_any->any_as_webstreamer_livestreamer_Create();
                        create_livestream(*create, cb);
                    }
                    break;
                    case webstreamer::Any_webstreamer_livestreamer_Destroy:
                    {
                        auto destroy = livestream_any->any_as_webstreamer_livestreamer_Destroy();
                        // message::livestream::destroy_t internal_destroy;
                        // flatbuffers::transform(*destroy, internal_destroy);
                        // on_livestream_destroy(internal_destroy);
                    }
                    break;
                    case webstreamer::Any_webstreamer_livestreamer_AddViewer:
                    {
                        auto add_endpoint = livestream_any->any_as_webstreamer_livestreamer_AddViewer();
                        // message::livestream::add_endpoint_t internal_add_endpoint;
                        // flatbuffers::transform(*add_endpoint, internal_add_endpoint);
                        // on_livestream_add_endpoint(internal_add_endpoint);
                    }
                    break;
                    case webstreamer::Any_webstreamer_livestreamer_RemoveViewer:
                    {
                        auto remove_endpoint = livestream_any->any_as_webstreamer_livestreamer_RemoveViewer();
                        // message::livestream::remove_endpoint_t internal_remove_endpoint;
                        // flatbuffers::transform(*remove_endpoint, internal_remove_endpoint);
                        // on_livestream_remove_endpoint(internal_remove_endpoint);
                    }
                    break;
                    default:
                    {
                        printf("-----Invalid message-----\n");
                        std::string reason("Invalid message");
                        cb(static_cast<int>(status_code::Bad_Request),
                           static_cast<void *>(const_cast<char *>(reason.c_str())), reason.size());
                    }
                    break;
                }
            }
        }

        void PipelineManager::create_livestream(const webstreamer::livestreamer::Create &message, const callback_fn &cb)
        {
            std::string stream_id(message.name()->c_str());
            if (is_livestream_created(stream_id))
            {
                //already exists
                std::string reason("The stream: " + stream_id + " is already existed!");
                cb(static_cast<int>(status_code::Conflict),
                   static_cast<void *>(const_cast<char *>(reason.c_str())), reason.size());
                return;
            }

            // create stream pipeline
            std::shared_ptr<LiveStream> livestream = std::make_shared<LiveStream>(stream_id);
            // set stream encoding
            const webstreamer::Endpoint *endpoint = message.source();
            for (auto it : *(endpoint->channel()))
            {
                std::string codec(((webstreamer::Channel *)it)->name()->c_str());
                if (codec == "video")
                {
                    livestream->video_encoding() = codec;
                }
                else if (codec == "audio")
                {
                    livestream->audio_encoding() = codec;
                }
                else
                {
                    std::string reason("The codec: " + codec + " is invalid!");
                    cb(static_cast<int>(status_code::Bad_Request),
                       static_cast<void *>(const_cast<char *>(reason.c_str())), reason.size());
                    return;
                }
            }
            // set rtsp source url
            livestream->rtsp_url() = *(endpoint->url()->c_str());

            // create source endpoint
            std::string endpoint_id(endpoint->name()->c_str());
            std::string endpoint_type(endpoint->protocol()->c_str());
            std::shared_ptr<Endpoint> ep = MakeEndpoint(endpoint_type, endpoint_id, livestream);

            // add endpoint to pipeline
            if (!livestream->add_endpoint(ep))
            {
                std::string reason("Add endpoint failed!");
                cb(static_cast<int>(status_code::Bad_Request),
                   static_cast<void *>(const_cast<char *>(reason.c_str())), reason.size());
                return;
            }

            // add pipeline to pipeline manager
            pipelines_.push_back(livestream);
            cb(0, NULL, 0);
            g_message("---------create_live_stream %s---------\n", stream_id.c_str());
        }
        void PipelineManager::destroy_livestream(const webstreamer::livestreamer::Destroy &message, const callback_fn &cb)
        {
            std::string stream_id(message.name()->c_str());
            auto it = std::find_if(pipelines_.begin(), pipelines_.end(), [&stream_id](std::shared_ptr<Pipeline> &pipeline) {
                return pipeline->id() == stream_id;
            });

            if (it == pipelines_.end())
            {
                std::string reason("LiveStream Not Found!");
                cb(static_cast<int>(status_code::Not_Found),
                   static_cast<void *>(const_cast<char *>(reason.c_str())), reason.size());
                return;
            }

            this->pipelines_.erase(it);

            cb(0, NULL, 0);
            g_message("---------delete_live_stream %s---------\n", stream_id.c_str());
        }
        void PipelineManager::add_endpoint_in_livestream(const webstreamer::livestreamer::AddViewer &message, const callback_fn &cb)
        {
        }
        void PipelineManager::remove_endpoint_in_livestream(const webstreamer::livestreamer::RemoveViewer &message, const callback_fn &cb)
        {
        }

        bool PipelineManager::is_livestream_created(const std::string &id)
        {
            auto it = std::find_if(pipelines_.begin(), pipelines_.end(), [&id](std::shared_ptr<Pipeline> &pipeline) {
                return pipeline->id() == id;
            });

            if (it != pipelines_.end())
            {
                return true;
            }

            return false;
        }
    }
}