#include <flatbuffers/adapter.hpp>
#include <message/endpoint.hpp>
#include <glib.h>

namespace libwebstreamer
{
	namespace flatbuffers
	{
		void transform(::webstreamer::Endpoint endpoint, const void * value, std::shared_ptr<message::common::Endpoint>& cxx_endpoint)
		{
			switch (endpoint)
			{
			case ::webstreamer::Endpoint::Endpoint_RtspClient:
			{
				const ::webstreamer::RtspClient * fb_endpoint = static_cast<const ::webstreamer::RtspClient *>(value);
				std::shared_ptr<message::endpoint::RtspClient> endpoint = std::make_shared<message::endpoint::RtspClient>();
				endpoint->id = fb_endpoint->base()->id()->str();
				endpoint->type = message::common::RTSP_CLIENT;
				endpoint->url = fb_endpoint->url()->str();

				cxx_endpoint = endpoint;
			}
			break;
			case ::webstreamer::Endpoint::Endpoint_Webrtc:
			{
				const ::webstreamer::Webrtc * fb_endpoint = static_cast<const ::webstreamer::Webrtc *>(value);
				std::shared_ptr<message::endpoint::Webrtc> endpoint = std::make_shared<message::endpoint::Webrtc>();
				endpoint->id = fb_endpoint->base()->id()->str();
				endpoint->type = message::common::WEBRTC;
				endpoint->group = fb_endpoint->group()->str();
				endpoint->connection = fb_endpoint->connection()->str();
				switch (fb_endpoint->sdp_role())
				{
				case ::webstreamer::webrtc::SdpRole::SdpRole_OFFER:
					endpoint->sdp_role = message::webrtc::SdpRole::OFFER;
					break;
				case ::webstreamer::webrtc::SdpRole::SdpRole_ANSWER:
					endpoint->sdp_role = message::webrtc::SdpRole::ANSWER;
					break;
				case ::webstreamer::webrtc::SdpRole::SdpRole_ANY:
					endpoint->sdp_role = message::webrtc::SdpRole::ANY;
					break;
				default:
					g_warn_if_reached();
					break;
				}

				switch (fb_endpoint->video_stream_mode())
				{
				case ::webstreamer::webrtc::StreamMode::StreamMode_SENDONLY:
					endpoint->video_mode = message::webrtc::StreamMode::SEND_ONLY;
					break;
				case ::webstreamer::webrtc::StreamMode::StreamMode_RECVONLY:
					endpoint->video_mode = message::webrtc::StreamMode::RECV_ONLY;
					break;
				case ::webstreamer::webrtc::StreamMode::StreamMode_SENDRECV:
					endpoint->video_mode = message::webrtc::StreamMode::SEND_RECV;
					break;
				default:
					g_warn_if_reached();
					break;
				}

				switch (fb_endpoint->audio_stream_mode())
				{
				case ::webstreamer::webrtc::StreamMode::StreamMode_SENDONLY:
					endpoint->audio_mode = message::webrtc::StreamMode::SEND_ONLY;
					break;
				case ::webstreamer::webrtc::StreamMode::StreamMode_RECVONLY:
					endpoint->audio_mode = message::webrtc::StreamMode::RECV_ONLY;
					break;
				case ::webstreamer::webrtc::StreamMode::StreamMode_SENDRECV:
					endpoint->audio_mode = message::webrtc::StreamMode::SEND_RECV;
					break;
				default:
					g_warn_if_reached();
					break;
				}

				cxx_endpoint = endpoint;
			}
			break;
			default:
				break;
			}
		}

		//fb - flatbuffer
		void transform(const webstreamer::LiveStreamCreate& fb_message, message::livestream::create_t& cxx_message)
		{
			//pipeline
			//pipeline.id
			cxx_message.pipeline.id = fb_message.pipeline()->id()->str();
			switch (fb_message.pipeline()->video_codec())//pipeline.video_codec
			{
			case ::webstreamer::VideoCodec::VideoCodec_H264:
				cxx_message.pipeline.video = message::common::VideoCodec::H264;
				break;
			case ::webstreamer::VideoCodec::VideoCodec_H265:
				cxx_message.pipeline.video = message::common::VideoCodec::H265;
				break;
			case ::webstreamer::VideoCodec::VideoCodec_VP8:
				cxx_message.pipeline.video = message::common::VideoCodec::VP8;
				break;
			default:
				g_warn_if_reached();
				break;
			}
			switch (fb_message.pipeline()->audio_codec())//pipeline.audio_codec
			{
			case ::webstreamer::AudioCodec::AudioCodec_OPUS:
				cxx_message.pipeline.audio = message::common::AudioCodec::OPUS;
				break;
			case ::webstreamer::AudioCodec::AudioCodec_PCMA:
				cxx_message.pipeline.audio = message::common::AudioCodec::PCMA;
				break;
			default:
				g_warn_if_reached();
				break;
			}

			//endpoint
			transform(fb_message.endpoint_type(), fb_message.endpoint(), cxx_message.endpoint);
		}

		void transform(const webstreamer::LiveStreamDestroy& fb_message, message::livestream::destroy_t& cxx_message)
		{
			cxx_message.id = fb_message.id()->str();
		}

		void transform(const webstreamer::LiveStreamAddEndpoint& fb_message, message::livestream::add_endpoint_t& cxx_message)
		{
			cxx_message.id = fb_message.id()->str();
			transform(fb_message.endpoint_type(), fb_message.endpoint(), cxx_message.endpoint);
		}

		void transform(const webstreamer::LiveStreamRemoveEndpoint& fb_message, message::livestream::remove_endpoint_t& cxx_message)
		{
			cxx_message.id = fb_message.id()->str();
			cxx_message.endpoint_id = fb_message.endpoint_id()->str();
		}

		void transform(const webstreamer::LiveStreamAddEndpoints& fb_message, message::livestream::add_endpoints_t& cxx_message)
		{
			cxx_message.id = fb_message.id()->str();
			auto endpoints_type = fb_message.endpoints_type();
			auto endpoints = fb_message.endpoints();
			
			for (int i = 0; i < (int)endpoints->size(); i++)
			{
				std::shared_ptr< message::common::Endpoint> endpoint;
				transform( endpoints_type->GetEnum<::webstreamer::Endpoint>(i), endpoints->GetAs<::webstreamer::RtspClient>(i), endpoint );
				cxx_message.endpoints.push_back(endpoint);
			}
		}

		void transform(const webstreamer::LiveStreamRemoveEndpoints& fb_message, message::livestream::remove_endpoints_t& cxx_message)
		{
			cxx_message.id = fb_message.id()->str();
			for (int i = 0; i < (int)fb_message.endpoint_ids()->size(); i++)
			{
				cxx_message.endpoint_ids.push_back(fb_message.endpoint_ids()->GetAsString(i)->str());
			}
		}
	}
}