#include <fbs/webstreamer_generated.h>
#include <framework/pipeline_manager.hpp>
// #include <glib.h>

namespace libwebstreamer
{
    namespace pipeline
    {
        namespace framewaork
        {
            void PipelineManager::call(const void *self, const void *context, const void *data, size_t size)
            {
                ::flatbuffers::Verifier verifier((const uint8_t *)data, size);
                if (webstreamer::VerifyLiveStreamAnyBuffer(verifier))
                {
                    auto livestream_any = webstreamer::GetLiveStreamAny((const uint8_t *)data);
                    switch (livestream_any->any_type())
                    {
                        case webstreamer::Any_LiveStreamCreate:
                        {
                            auto create = livestream_any->any_as_LiveStreamCreate();
                            message::livestream::create_t internal_create;
                            flatbuffers::transform(*create, internal_create);
                            on_livestream_create(internal_create);
                        }
                        break;
                        case webstreamer::Any_LiveStreamDestroy:
                        {
                            auto destroy = livestream_any->any_as_LiveStreamDestroy();
                            message::livestream::destroy_t internal_destroy;
                            flatbuffers::transform(*destroy, internal_destroy);
                            on_livestream_destroy(internal_destroy);
                        }
                        break;
                        case webstreamer::Any_LiveStreamAddEndpoint:
                        {
                            auto add_endpoint = livestream_any->any_as_LiveStreamAddEndpoint();
                            message::livestream::add_endpoint_t internal_add_endpoint;
                            flatbuffers::transform(*add_endpoint, internal_add_endpoint);
                            on_livestream_add_endpoint(internal_add_endpoint);
                        }
                        break;
                        case webstreamer::Any_LiveStreamRemoveEndpoint:
                        {
                            auto remove_endpoint = livestream_any->any_as_LiveStreamRemoveEndpoint();
                            message::livestream::remove_endpoint_t internal_remove_endpoint;
                            flatbuffers::transform(*remove_endpoint, internal_remove_endpoint);
                            on_livestream_remove_endpoint(internal_remove_endpoint);
                        }
                        break;
                        case webstreamer::Any_LiveStreamAddEndpoints:
                        {
                            auto add_endpoints = livestream_any->any_as_LiveStreamAddEndpoints();
                            message::livestream::add_endpoints_t internal_add_endpoints;
                            flatbuffers::transform(*add_endpoints, internal_add_endpoints);
                            on_livestream_add_endpoints(internal_add_endpoints);
                        }
                        break;
                        case webstreamer::Any_LiveStreamRemoveEndpoints:
                        {
                            auto remove_endpoints = livestream_any->any_as_LiveStreamRemoveEndpoints();
                            message::livestream::remove_endpoints_t internal_remove_endpoints;
                            flatbuffers::transform(*remove_endpoints, internal_remove_endpoints);
                            on_livestream_remove_endpoints(internal_remove_endpoints);
                        }
                        break;
                        default:
                            g_warn_if_reached();
                            break;
                    }
                }
            }

            void PipelineManager::on_livestream_create(const message::livestream::create_t &message)
            {
                if (is_livestream_created(message.pipeline.id))
                {
                    //TODO: Exception Feedback
                    return;
                }
            }
            void PipelineManager::on_livestream_destroy(const message::livestream::destroy_t &message)
            {
            }
            void PipelineManager::on_livestream_add_endpoint(const message::livestream::add_endpoint_t &message)
            {
            }
            void PipelineManager::on_livestream_remove_endpoint(const message::livestream::remove_endpoint_t &message)
            {
            }
            void PipelineManager::on_livestream_add_endpoints(const message::livestream::add_endpoints_t &message)
            {
            }
            void PipelineManager::on_livestream_remove_endpoints(const message::livestream::remove_endpoints_t &message)
            {
            }

            bool PipelineManager::is_livestream_created(const std::string &id)
            {
                auto it = std::find_if(pipelines.begin(), pipelines.end(), [&id](std::shared_ptr<Pipeline> &pipeline) {
                    return pipeline->id() == id;
                });

                if (it != pipelines.end())
                {
                    return true;
                }

                return false;
            }
        }
    }
}