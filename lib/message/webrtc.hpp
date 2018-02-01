#ifndef LIBWEBSTREAMER_MESSAGE_WEBRTC_HPP
#define LIBWEBSTREAMER_MESSAGE_WEBRTC_HPP

namespace libwebstreamer
{
	namespace message
	{
		namespace webrtc
		{

			enum SdpRole
			{
				OFFER,
				ANSWER,
				ANY
			};

			enum StreamMode
			{
				SEND_ONLY,
				RECV_ONLY,
				SEND_RECV,
				NONE,
			};

		}
	}
}

#endif