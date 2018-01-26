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