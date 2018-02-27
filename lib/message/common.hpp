#ifndef LIBWEBSTREAMER_MESSAGE_COMMON_HPP
#define LIBWEBSTREAMER_MESSAGE_COMMON_HPP

#include <string>
#include "webrtc.hpp"

namespace libwebstreamer
{
	namespace message
	{
		namespace common
		{
			//Endpoint
			enum EndpointType
			{
				RTSP_CLIENT,
				WEBRTC
			};
			
			struct Endpoint
			{
				std::string id;
				EndpointType type;
			};

			//Pipeline
			enum VideoCodec
			{
				H264,
				H265,
				VP8
			};

			enum AudioCodec
			{
				OPUS,
				PCMA
			};
			
			struct Pipeline
			{
				std::string id;
				VideoCodec video;
				AudioCodec audio;
			};

			struct error_t
			{
				int8_t code;
				std::string reason;
			};
		}
	}
}

#endif