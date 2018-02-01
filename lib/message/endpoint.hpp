#ifndef LIBWEBSTREAMER_MESSAGE_ENDPOINT_HPP
#define LIBWEBSTREAMER_MESSAGE_ENDPOINT_HPP

#include "common.hpp"

namespace libwebstreamer
{
	namespace message
	{
		namespace endpoint
		{
			struct RtspClient : public common::Endpoint
			{
				std::string url;
			};

			struct Webrtc : public common::Endpoint
			{
				std::string group;
				std::string connection;
				webrtc::SdpRole sdp_role;
				webrtc::StreamMode audio_mode;
				webrtc::StreamMode video_mode;
			};

		}
	}
}

#endif