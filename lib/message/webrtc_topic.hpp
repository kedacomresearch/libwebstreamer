#ifndef LIBWEBSTREAMER_MESSAGE_WEBRTC_TOPICS_HPP
#define LIBWEBSTREAMER_MESSAGE_WEBRTC_TOPICS_HPP

#include "webrtc.hpp"
#include <string>
#include <vector>
#include <memory>

namespace libwebstreamer
{
	namespace message
	{
		namespace webrtc
		{
			namespace topic
			{
				enum Category
				{
					SDP,
					STATUS,
				};
				
				struct Endpoint
				{
					webrtc::SdpRole sdp_role;
					std::string group;
					std::string connection;
				};
				
				struct SubscribeItem
				{
					Endpoint endpoint;
					std::vector<topic::Category> topics;
				};

				struct Topic
				{
					Category category;
				};
				
				struct Sdp : public Topic
				{
					std::string content;
				};

				struct Status : public Topic
				{
					std::string content;//"connecting", "connected", "disconnected"
				};

				struct subscribe_t
				{
					std::vector<SubscribeItem> items;
				};

				struct notification_t
				{
					Endpoint endpoint;
					std::vector<std::shared_ptr<Topic>> topics;
				};

				struct push_t
				{
					Endpoint endpoint;
					std::vector<std::shared_ptr<Topic>> topics;
				};
			}
		}
	}
}


#endif