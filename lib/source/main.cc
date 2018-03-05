#include <stdio.h>
#include <flatbuffers/flatbuffers.h>


#include <node_plugin_interface.h>
#include <dispatcher.hpp>



#define __VERSION__ 0.1.0

static const char *default_res[] = {"Callback successfully!",
                                    "Callback failed!"};

static void init(const void *self, const void *data, size_t size,
                 void (*cb)(const void *self, int status, char *msg))
{
    //TODO:
    //data is JSON string (utf8)
    //do as your needs
    if (data)
    {
        //do parse
        std::string param((const char *)data, size);
        printf("init param <%s>\n", param.c_str());
        //parse webstreamer parameter
        //todo...
    }
    printf("gstreamer init!\n");
    libwebstreamer::initialize();
    printf("gstreamer init done.\n");

    if (cb)
    {
        cb(self, 0, ">>>>>Initialize done!<<<<<");
        //error callback
        // cb(self, 1 ,"Initalize error!");
    }
}

static void call(const void *self, const void *context,
                 const void *data, size_t size)
{
    static int counter = 0;
    node_plugin_interface_t *iface = (node_plugin_interface_t *)self;
    if (iface->call_return)
    {
        libwebstreamer::call(data, size,
                             [iface, self, context](int status, void *cb_data, size_t cb_szie) {
                                 if (cb_data == NULL)
                                 {
                                     if (status == 0)
                                     {
                                         iface->call_return(self, context, default_res[0],
                                                            strlen(default_res[0]) + 1, status, NULL, NULL);
                                         return;
                                     }
                                     else
                                     {
                                         iface->call_return(self, context, default_res[1],
                                                            strlen(default_res[1]) + 1, status, NULL, NULL);
                                         return;
                                     }
                                 }
                                 iface->call_return(self, context, cb_data, cb_szie, status, NULL, NULL);
                             });
    }
    else
    {
        printf("Warning: No callback!\n");
        // todo...
    }

    counter++;
    if (iface->notify)
    {
        char log[256];
        sprintf(log, "* %d request has been procced.", counter);
        iface->notify(self, log, strlen(log) + 1, NULL, NULL);
    }
}

static void terminate(const void *self, void (*cb)(const void *self, int status, char *msg))
{
    libwebstreamer::terminate();
    printf("gstreamer quit.\n");
    if (cb)
    {
        cb(self, 0, ">>>>>Terminate done!<<<<<");
        //error callback
        // cb(self, 1 ,"Terminate error!");
    }
}

NODE_PLUGIN_IMPL(__VERSION__, init, call, terminate)


/////////////////////////////////////////////////////////////////////////////////////////////
