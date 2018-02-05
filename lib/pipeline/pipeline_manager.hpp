#ifndef LIBWEBSTREAMER_PIPELINE_MANAGER_HPP
#define LIBWEBSTREAMER_PIPELINE_MANAGER_HPP

#include <message/livestream.hpp>

namespace libwebstreamer
{
	namespace pipeline
	{
		class PipelineManager
		{
		public:
			void on_nodejs_call(const void* self, const void* context, const void* data, size_t size);
		
		private://LiveStream
			void on_livestream_create(const message::livestream::create_t& message);
			void on_livestream_destroy(const message::livestream::destroy_t& message);
			void on_livestream_add_endpoint(const message::livestream::add_endpoint_t& message);
			void on_livestream_remove_endpoint(const message::livestream::remove_endpoint_t& message);
			void on_livestream_add_endpoints(const message::livestream::add_endpoints_t& message);
			void on_livestream_remove_endpoints(const message::livestream::remove_endpoints_t& message);
		};
	}
}

#endif