#ifndef LIBWEBSTREAMER_FLATBUFFERS_ADAPTER_HPP
#define LIBWEBSTREAMER_FLATBUFFERS_ADAPTER_HPP

#include <flatbuffers/webstreamer_generated.h>
#include <message/livestream.hpp>

namespace libwebstreamer
{
	namespace flatbuffers
	{
		void transform(const webstreamer::LiveStreamCreate& fb_message, message::livestream::create_t& cxx_message);
		void transform(const webstreamer::LiveStreamDestroy& fb_message, message::livestream::destroy_t& cxx_message);
		void transform(const webstreamer::LiveStreamAddEndpoint& fb_message, message::livestream::add_endpoint_t& cxx_message);
		void transform(const webstreamer::LiveStreamRemoveEndpoint& fb_message, message::livestream::remove_endpoint_t& cxx_message);
		void transform(const webstreamer::LiveStreamAddEndpoints& fb_message, message::livestream::add_endpoints_t& cxx_message);
		void transform(const webstreamer::LiveStreamRemoveEndpoints& fb_message, message::livestream::remove_endpoints_t& cxx_message);
	}
}

#endif