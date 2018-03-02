#include <framework/endpoint.hpp>
#include <application/endpoint/rtspclient.hpp>
#include <application/pipeline/livestream.hpp>

namespace libwebstreamer
{
    namespace framework
    {
        Endpoint::Endpoint(const std::string &id, const std::string &type, const std::shared_ptr<Pipeline> pipeline_owner)
            : id_(id)
            , type_(type)
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
            switch (get_endpoint_type(type))
            {
                case EndpointType::RTSP_CLIENT:
                {
                    typedef libwebstreamer::application::endpoint::RtspClient rtspclient;
                    typedef libwebstreamer::application::pipeline::LiveStream livestream;
                    std::shared_ptr<rtspclient> ep = std::make_shared<rtspclient>(id, type, pipeline_owner);
                    const std::string rtsp_url = static_cast<livestream *>(pipeline_owner.get())->rtsp_url();
                    ep->initialize(rtsp_url);
                    return ep;
                }
                // case StringValue::WEBRTC:
                //     return std::make_shared<webstreamer::pipeline::endpoint::RtspServer>(
                //         *static_cast<webstreamer::transport::mediaservice::RtspServer *>(&endpoint));
                default:
                    return NULL;
            }
        }
    }
}