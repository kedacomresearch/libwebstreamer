#ifndef LIBWEBSTREAMER_FRAMEWORK_PIPELINE_MANAGER_HPP
#define LIBWEBSTREAMER_FRAMEWORK_PIPELINE_MANAGER_HPP

#include <message/livestream.hpp>
#include "pipeline.hpp"
/**
* ,-----------------, 1               * ,---------------, 1    shared_ptr   * ,---------------,
* | PipelineManager |------------------>|   Pipeline    |-------------------->|    Endpoint   |
* '-----------------'   shared_ptr      '---------------'                     '---------------'
*                                                   ^._1_ _ _ _ _ _ _ _ _ _ _ _1_/    1 .^
*                                                            weak_ptr                  /
*                                               ,-----------------,                   /           
*                                               |   WebrtcManager |_1_ _ _ _ _ _ _ _ /
*                                               '-----------------'     weak_ptr
*/

namespace libwebstreamer
{
    namespace pipeline
    {
        namespace framewaork
        {
            class PipelineManager
            {
            public:
                void call(const void *self, const void *context, const void *data, size_t size);

            private://LiveStream
                void on_livestream_create(const message::livestream::create_t &message);
                void on_livestream_destroy(const message::livestream::destroy_t &message);
                void on_livestream_add_endpoint(const message::livestream::add_endpoint_t &message);
                void on_livestream_remove_endpoint(const message::livestream::remove_endpoint_t &message);
                void on_livestream_add_endpoints(const message::livestream::add_endpoints_t &message);
                void on_livestream_remove_endpoints(const message::livestream::remove_endpoints_t &message);

            private:
                bool is_livestream_created(const std::string &id);

            private:
                std::vector<std::shared_ptr<Pipeline>> pipelines;
            };
        }
    }
}

#endif