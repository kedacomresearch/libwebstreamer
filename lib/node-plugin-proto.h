
#ifndef _NODE_PLUGIN_FUNCTION_PROTOTYPE_DEF_H_
#define _NODE_PLUGIN_FUNCTION_PROTOTYPE_DEF_H_


#ifdef __cplusplus
extern "C"{
#endif


//
typedef void(*node_plugin_finalize_fn)(
	const void* self,
	void* finalize_data,
	void* finalize_hint);

typedef void(*node_plugin_call_return_fn)(const void* self, const void* context,
	const void* data, size_t size,
	int status,
	node_plugin_finalize_fn finalizer,
	void* hint);

typedef void(*node_plugin_notify_fn)(const void* self, const void* data, size_t size,
	node_plugin_finalize_fn finalizer,
	void* hint);
//
//
//typedef void(*node_plugin_callback_function_t)(
//	const void* context, /*context which set in plugin, or NULL for notification*/
//	const void* data, size_t size, /*to pass to Javascript as node::Buffer*/
//	int   status, /*optional ,normally 0 mean success */
//	NODE_PLUGIN_FINALIZE finalize,/*optional callback to call the data collected*/
//	void* hint /*optional hint to pass to the finalize callback during collection. */
//	);

struct _node_plugin_interface_t {

	//set by plugin
	void (* init     )(const void* self, const void* data, size_t size);
	void (* call     )(const void* self, const void* context, 
		               const void* data, size_t size);
	void (* terminate)(const void* self, void(*done)(const void* self));

	//set by addon
	node_plugin_call_return_fn call_return;
	node_plugin_notify_fn      notify;

	//
	void* addon_; //node addon 
};

typedef struct _node_plugin_interface_t node_plugin_interface_t;

typedef node_plugin_interface_t* (*node_plugin_interface_initialize_fn)(
								   void* addon,
								   node_plugin_call_return_fn call_return,
								   node_plugin_notify_fn      notify );

typedef void (*node_plugin_interface_terminate_fn)(node_plugin_interface_t* iface);

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
/**
	example
	static void init(const void* self, const void* data, size_t size)
	{
		//TODO:
		//data is JSON string (utf8)
		//do as your needs
		....
	}

	static void call(const void* self, const void* context,
	const void* data, size_t size)
	{
	   ......
	}

	static void terminate(const void* self, void(*done)(const void* self))
	{
		....
		if (done) {
		done(self);
		}
	}
	NODE_PLUGIN_IMPL( init , call , terminate)

*/
#define NODE_PLUGIN_IMPL( _init_ , _call_ , _terminate_)            \
NODE_PLUGIN_SYMBOL                                                  \
node_plugin_interface_t* node_plugin_interface_initialize(          \
	void* addon,                                                    \
	node_plugin_call_return_fn call_return,                         \
	node_plugin_notify_fn      notify)                              \
{                                                                   \
	node_plugin_interface_t* iface = (node_plugin_interface_t*)     \
		malloc(sizeof(node_plugin_interface_t));                    \
	memset(iface, 0, sizeof(node_plugin_interface_t));              \
	iface->addon_ = addon;                                          \
	iface->call_return = call_return;                               \
	iface->notify = notify;                                         \
		                                                            \
	/* set plugin functions */                                      \
	iface->init = _init_;                                           \
	iface->call = _call_;                                           \
	iface->terminate = _terminate_;	                                \
	return iface;					                                \
}															   		\
																	\
NODE_PLUGIN_SYMBOL													\
void node_plugin_interface_terminate(node_plugin_interface_t* iface)\
{																	\
	free(iface);													\
}

#endif