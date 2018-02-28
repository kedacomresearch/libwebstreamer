#ifndef LIBWEBSTREAMER_FRAMEWORK_ENDPOINT_HPP
#define LIBWEBSTREAMER_FRAMEWORK_ENDPOINT_HPP

#include <message/common.hpp>

namespace libwebstreamer
{
    namespace pipeline
    {
        namespace framework
        {
            class Pipeline;
            class Endpoint
            {
            public:
                Endpoint(const message::common::Endpoint &params);
                virtual ~Endpoint();

            private:
                std::weak_ptr<Pipeline> pipeline;//owner pipeline
            };
        }
    }
}


#endif