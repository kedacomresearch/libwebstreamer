
#ifndef _NODE_PLUGIN_FUNCTION_PROTOTYPE_DEF_H_
#define _NODE_PLUGIN_FUNCTION_PROTOTYPE_DEF_H_


#ifdef __cplusplus
extern "C"{
#endif

/**
 * function called by Javascript(addon) which implement in plugin (c/c++)
*/

/*node_plugin_call*/
typedef void (* NODE_PLUGIN_CALL)(
                const void* data, size_t size, /*Javascript passed node::Buffer*/ 
                void* context /* if context == NULL,there will be no callback called*/
		);
				 
typedef void(*NODE_PLUGIN_FINALIZE)(
	         const void* env,
	         void* finalize_data,
		     void* finalize_hint);


typedef void (* NODE_PLUGIN_CALLBACK )(
				const void* context, /*context which set in plugin, or NULL for notification*/
				const void* data, size_t size, /*to pass to Javascript as node::Buffer*/
				int   status, /*optional ,normally 0 mean success */
	            NODE_PLUGIN_FINALIZE finalize,/*optional callback to call the data collected*/
				void* hint /*optional hint to pass to the finalize callback during collection. */
		);

/*node_plugin_init*/
typedef int(*NODE_PLUGIN_INIT)(const void* data, size_t size /*JSON */,
	const void* context,
	NODE_PLUGIN_CALLBACK callback, /* callback for plugin_call. */
	NODE_PLUGIN_CALLBACK notify    /* notify (no call).*/
	);

/*node_plugin_terminate*/
typedef void(*NODE_PLUGIN_TERMINATE)(void(*done)());


typedef void(*node_plugin_callback_function_t)(
	const void* context, /*context which set in plugin, or NULL for notification*/
	const void* data, size_t size, /*to pass to Javascript as node::Buffer*/
	int   status, /*optional ,normally 0 mean success */
	NODE_PLUGIN_FINALIZE finalize,/*optional callback to call the data collected*/
	void* hint /*optional hint to pass to the finalize callback during collection. */
	);

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

struct _node_plugin_interface_t {

	//set by plugin
	void (* init     )(const void* self, const void* data, size_t size, 
					   void(*cb)(const void* self, int status, char *msg));
	void (* call     )(const void* self, const void* context, 
		               const void* data, size_t size);
	void (* terminate)(const void* self, void(*cb)(const void* self, int status, char *msg));

	//set by addon
	node_plugin_call_return_fn call_return;
	node_plugin_notify_fn      notify;
	char* version;

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
	static void init(const void *self, const void *data, size_t size, void (*cb)(const void *self, int status, char *msg))
	{
		//TODO:
		//data is JSON string (utf8)
		//do as your needs

		if (cb)
		{
			cb(self, 0, ">>>>>Initialize done!<<<<<");
			//error callback
			// cb(self, 1 ,"Initalize error!");
		}
	}

	static void call(const void* self, const void* context,
	const void* data, size_t size)
	{
	   ......
	}

	static void terminate(const void *self, void (*cb)(const void *self, int status, char *msg))
	{
		if (cb)
		{
			cb(self, 0, ">>>>>Terminate done!<<<<<");
			//error callback
			// cb(self, 1 ,"Terminate error!");
		}
	}
	NODE_PLUGIN_IMPL( 0.1.0, init , call , terminate)

*/
#define NODE_PLUGIN_IMPL( _VERSION, _init_ , _call_ , _terminate_)            \
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
	iface->version = #_VERSION;                                     \
	return iface;													\
}																	\
																	\
NODE_PLUGIN_SYMBOL													\
void node_plugin_interface_terminate(node_plugin_interface_t* iface)\
{																	\
	free(iface);													\
}

#endif