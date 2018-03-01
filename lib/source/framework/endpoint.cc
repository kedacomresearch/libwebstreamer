#include <framework/endpoint.hpp>
#include <application/endpoint/rtspclient.hpp>

namespace libwebstreamer
{
    namespace framework
    {
        static std::map<std::string, EndpointType> type_map = {{"rtsp", EndpointType::RTSP},
                                                               {"webrtc", EndpointType::WEBRTC}};

        Endpoint::Endpoint(const std::string &id, const std::string &type, const std::shared_ptr<Pipeline> pipeline_owner)
            : id_(id)
            , type_(type_map[type])
            , pipeline_owner_(pipeline_owner)
        {
        }

        // void Endpoint::add_to_pipeline()
        // {
        //     on_add_to_pipeline();
        // }

        // void Endpoint::remove_from_pipeline()
        // {
        //     on_remove_from_pipeline();
        // }



        std::shared_ptr<Endpoint> MakeEndpoint(const std::string &type, const std::string &id, const std::shared_ptr<Pipeline> pipeline_owner)
        {
            switch (type_map[type])
            {
                case EndpointType::RTSP:
                    return std::make_shared<libwebstreamer::application::endpoint::RtspClient>(id, type, pipeline_owner);
                // case StringValue::WEBRTC:
                //     return std::make_shared<webstreamer::pipeline::endpoint::RtspServer>(
                //         *static_cast<webstreamer::transport::mediaservice::RtspServer *>(&endpoint));
                default:
                    return NULL;
            }
        }
    }
}