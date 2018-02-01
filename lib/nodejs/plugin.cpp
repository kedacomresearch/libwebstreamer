//////////////////////////////////
#include "node-plugin-proto.h"
#include <libwebstreamer.hpp>

#include <nodejs/adapter/dispatcher.hpp>

#include <string.h>
USING_LIBWEBSTREAMER_NS

static void init(const void* self, const void* data, size_t size)
{
	//TODO:
	//data is JSON string (utf8)
	//do as your needs
	LIBWEBSTREAMER_PREFIX initialize( (const char*)data,size);
}

static void call(const void* self, const void* context,
	const void* data, size_t size)
{
	nodejs::adapter::on_nodejs_call(self, context, data, size);
}

static void terminate(const void* self, void(*done)(const void* self))
{
	LIBWEBSTREAMER_PREFIX terminate();
	if (done) {
		done(self);
	}
}


NODE_PLUGIN_IMPL(init,call,terminate)