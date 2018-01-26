#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <glib.h>

#define GST_EXPORTS
#include <gst/gst.h>


//#include "libwebstreamer.hpp"
//#include "dispatcher.hpp"


//
//
//#define EXTERN_C extern "C"
//
//
//static	NODE_PLUGIN_CALLBACK g_callback = NULL;
//static	NODE_PLUGIN_CALLBACK g_notify = NULL;
//
////@NODE_PLUGIN_CALL
//EXTERN_C GST_EXPORT
//void plugin_call(const void* data, size_t size, void* context)
//{
//	::libwebstreamer::initialize();
//	::libwebstreamer::dispatcher::on_call(data, size, context);
//}
//
////@NODE_PLUGIN_SET_CALLBACK
//EXTERN_C GST_EXPORT
//void plugin_set_callback(
//	NODE_PLUGIN_CALLBACK cb,
//	NODE_PLUGIN_CALLBACK notify)
//{
//	g_callback = cb;
//}

//
//namespace libwebstreamer
//{
//	namespace dispatcher
//	{
//		void on_callback(const void* data, size_t size, void* context)
//		{
//			static int n = 0;
//			n++;
//			char* buf = (char*)malloc(size + 64);
//			memset(buf, 0, size + 64);
//			sprintf(buf, "==>%d %s", n, data);
//			size_t len = strlen(buf);
//			if (context && g_callback)
//			{
//	
//				g_callback(context, buf, len + 1, 0, NULL, NULL);
//			}
//
//			if (context && g_notify)
//			{
//				buf[0] = '-';
//				g_notify(context, buf, len + 1, 0, NULL, NULL);
//			}
//
//			free(buf);
//		}
//	}
//}










//////////////////////////////////
#include "node-plugin-proto.h"
#include "webstreamer.hpp"
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
}

static void terminate(const void* self, void(*done)(const void* self))
{
	LIBWEBSTREAMER_PREFIX terminate();
	if (done) {
		done(self);
	}
}


NODE_PLUGIN_IMPL(init,call,terminate)