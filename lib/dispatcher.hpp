#ifndef LIBWEBSTREAMER_DISPATCHER_DISPATCHER_HPP
#define LIBWEBSTREAMER_DISPATCHER_DISPATCHER_HPP

namespace libwebstreamer
{
	namespace dispatcher
	{
		//void on_call(const void* data, size_t size, void* context);
		//void on_callback(const void* data, size_t size, void* context);
		void on_plugin_call(const void* self, const void* context, const void* data, size_t size);

	}
}

#endif