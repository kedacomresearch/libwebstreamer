#ifndef LIBWEBSTREAMER_NODEJS_ADAPTER_DISPATCHER_HPP
#define LIBWEBSTREAMER_NODEJS_ADAPTER_DISPATCHER_HPP

namespace libwebstreamer
{
	namespace nodejs
	{
		namespace adapter
		{
			void on_plugin_call(const void* self, const void* context, const void* data, size_t size);
		}
	}
}

#endif