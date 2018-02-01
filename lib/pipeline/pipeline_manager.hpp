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
			void on_call(const void* self, const void* context, const void* data, size_t size);
		
		private://LiveStream
			void on_livestream_create();
			void on_livestream_destroy();
			void on_livestream_add_endpoint();
			void on_livestream_remove_endpoint();
			void on_livestream_add_endpoints();
			void on_livestream_remove_endpoints();
		};
	}
}

#endif