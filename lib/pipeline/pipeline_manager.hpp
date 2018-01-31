#ifndef LIBWEBSTREAMER_PIPELINE_MANAGER_HPP
#define LIBWEBSTREAMER_PIPELINE_MANAGER_HPP

namespace libwebstreamer
{
	namespace pipeline
	{
		class PipelineManager
		{
		public:
			void on_call(const void* self, const void* context, const void* data, size_t size);
		};
	}
}

#endif