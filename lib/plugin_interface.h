
#ifndef _NODE_PLUGIN_INTERFACE_H_
#define _NODE_PLUGIN_INTERFACE_H_
#include <malloc.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#define PLUGIN_CALL_OK 0


typedef struct plugin_buffer_t
{
	void*  data;
	size_t size;

	void   (*release)( struct plugin_buffer_t* self); 
	void   (*move)(struct plugin_buffer_t* self, struct plugin_buffer_t* dest);
	void*  context;
}plugin_buffer_t;

static void _default_plugin_buffer_move(plugin_buffer_t* self, plugin_buffer_t* dest)
{
	memcpy(dest, self, sizeof(plugin_buffer_t));
	memset(self, 0,sizeof(plugin_buffer_t));
}

static void _default_plugin_buffer_release(plugin_buffer_t* self)
{
	if (self->data) {
		free(self->data);
	}
	memset(self, 0, sizeof(plugin_buffer_t));
}

inline static int plugin_buffer_safe_move(plugin_buffer_t* src, plugin_buffer_t* dest)
{
	if (!src || !dest) return 0;

	if (src->move) {
		src->move(src, dest);
	}
	else {
		if (src->data && src->size) {
			dest->data = malloc(src->size);
			dest->size = src->size;
			memcpy(dest->data, src->data, dest->size);
			dest->move = _default_plugin_buffer_move;
			dest->release = _default_plugin_buffer_release;
		}
		else {
			memset(dest, 0, sizeof(plugin_buffer_t));//null
		}

		if (src->release) {
			src->release(src);
		}
	}
	return 1;
}

inline static void plugin_buffer_string_set(plugin_buffer_t* src, const char* message) {
	src->data = _strdup(message);
	src->size = strlen(message);
	
	src->move = _default_plugin_buffer_move;
	src->release = _default_plugin_buffer_release;
}

typedef void(*plugin_callback_fn)(const void* self,
	const void* context, int status,
	plugin_buffer_t*    data );

typedef void(*plugin_notify_fn)(
	const void* self,
	struct plugin_buffer_t*    data,
	struct plugin_buffer_t*    meta);


typedef struct plugin_interface_t {

	//set by plugin
	void(*init)(const void* self,
		const void*               context,
		const struct plugin_buffer_t*    data,
		plugin_callback_fn        callback);

	void(*call)(const void* self,
		const void*               context,
		struct plugin_buffer_t*          data,
		struct plugin_buffer_t*          meta,
		plugin_callback_fn        callback);

	void(*terminate)(const void* self,
		const void*               context,
		plugin_callback_fn        callback);

	//set by host language
	plugin_notify_fn      notify;

	const char* version;
	const void* context;

	//private
	void* instance;// the plugin application instance
}plugin_interface_t;



typedef plugin_interface_t* (*plugin_interface_initialize_fn)(
								   void* context,//host language context
								   plugin_notify_fn      notify );

typedef void (*plugin_interface_terminate_fn)(plugin_interface_t* iface);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#define _NODE_PLUGIN_DLLEXPORT __declspec(dllexport)
#else
#define _NODE_PLUGIN_DLLEXPORT  
#endif

#ifdef __cplusplus
#define NODE_PLUGIN_SYMBOL extern "C" _NODE_PLUGIN_DLLEXPORT
#else
#define NODE_PLUGIN_SYMBOL _NODE_PLUGIN_DLLEXPORT
#endif



#define PLUGIN_INTERFACE( _VERSION, _init_ , _call_ , _terminate_)  \
NODE_PLUGIN_SYMBOL                                                  \
plugin_interface_t* plugin_interface_initialize(                    \
	void* context,                                                  \
	plugin_notify_fn      notify)                                   \
{                                                                   \
	plugin_interface_t* iface = (plugin_interface_t*)               \
		malloc(sizeof( plugin_interface_t));                        \
	memset(iface, 0, sizeof( plugin_interface_t));                  \
	iface->context = context;                                       \
	iface->notify = notify;                                         \
		                                                            \
	/* set plugin functions */                                      \
	iface->init = _init_;                                           \
	iface->call = _call_;                                           \
	iface->terminate = _terminate_;	                                \
	iface->version   = _VERSION;                                    \
    iface->instance  = NULL;                                        \
	return iface;													\
}																	\
																	\
NODE_PLUGIN_SYMBOL													\
void plugin_interface_terminate(plugin_interface_t* iface)          \
{																	\
	free(iface);													\
}

#endif