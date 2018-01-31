#include <memory>
#include <glib.h>
#include "libwebstreamer.hpp"
#include "dispatcher.hpp"
#include <pipeline/pipeline_manager.hpp>

namespace libwebstreamer
{
	namespace dispatcher
	{
		static std::unique_ptr<GAsyncQueue, decltype(&g_async_queue_unref)> call_queue(g_async_queue_new(), &g_async_queue_unref);
		static std::unique_ptr<pipeline::PipelineManager> pipeline_manager(new pipeline::PipelineManager());
		static gboolean on_pipeline_manager_dispatch(gpointer pipeline_manager);
		
		struct call_t
		{
			call_t(const void* self, const void* context, const void* data, size_t size)
				: self_(self), 
				  context_(context), 
				  data_(data), 
				  size_(size)
			{
			}

			const void* self_;
			const void* context_;
			const void* data_;
			size_t size_;
		};

		void on_plugin_call(const void* self, const void* context, const void* data, size_t size)
		{
			call_t * call = new call_t(self, context, data, size);
			
			g_async_queue_push(call_queue.get(), call);

			dispatch(on_pipeline_manager_dispatch, pipeline_manager.get());

		}

		gboolean on_pipeline_manager_dispatch(gpointer pipeline_manager)
		{
			call_t * call = (call_t *)g_async_queue_pop(call_queue.get());
			static_cast<pipeline::PipelineManager*>(pipeline_manager)->on_call(call->self_, call->context_, call->data_, call->size_);
			delete call;

			return G_SOURCE_REMOVE;
		}
	}
}

//#include "webstreamer.hpp"
//#include "dispatcher.hpp"
//#include <memory>
//#include <glib.h>

//namespace libwebstreamer
//{
//	namespace dispatcher
//	{
//		static std::unique_ptr<GAsyncQueue, decltype(&g_async_queue_unref)> _data_queue(g_async_queue_new(), &g_async_queue_unref);
//		static std::unique_ptr<GAsyncQueue, decltype(&g_async_queue_unref)> _size_queue(g_async_queue_new(), &g_async_queue_unref);
//		static std::unique_ptr<GAsyncQueue, decltype(&g_async_queue_unref)> _context_queue(g_async_queue_new(), &g_async_queue_unref);
//		
//		static gboolean on_call_pipeline_manager(gpointer manager);
//
//		void on_call(const void* data, size_t size, void* context)
//		{
//			g_print("@on_call:=--data:%d,size:%d,context:%d---------\n", data, size, context);
//
//			g_async_queue_push(_context_queue.get(), context);
//			g_async_queue_push(_data_queue.get(), (gpointer)data);
//			g_async_queue_push(_size_queue.get(), (gpointer)size);
//
//			schedule_with_user_data(on_call_pipeline_manager, NULL);
//		}
//
//		gboolean on_call_pipeline_manager(gpointer manager)
//		{
//			const void* data = g_async_queue_pop(_data_queue.get()); 
//			size_t size = (size_t)g_async_queue_pop(_size_queue.get());
//			void* context = g_async_queue_pop(_context_queue.get());
//			g_print("@on_call_pipeline_manager:=--data:%d,size:%d,context:%d---------\n", data, size, context);
//			on_callback(data, size, context);
//
//			return G_SOURCE_REMOVE;
//		}
//	}
//}