/*
 * Copyright 2018 KEDACOM Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <gst/gst.h>

#include "nlohmann/json.hpp"
#include "./webstreamer.h"

void clear_global_webstreamer_instance();
WebStreamer* get_webstreamer_instance();
void set_webstreamer_instance(WebStreamer* inst);

// TODO(Mingyi): lock for global share MainLoop.
static GThread      *_main_thread = NULL;

GMainLoop*     WebStreamer::main_loop = NULL;;
GMainContext*  WebStreamer::main_context = NULL;

using json = nlohmann::json;

static gboolean main_loop_init(GAsyncQueue* queue)
{
    g_async_queue_push(queue, g_strdup("ready"));
    return G_SOURCE_REMOVE;
}



gpointer WebStreamer::MainloopEntry(Promise* promise) {
    WebStreamer* This = promise->webstreamer();

    This->MainLoop(promise);
    return NULL;
}
void WebStreamer::MainLoop(Promise* promise)
{
    WebStreamer::main_context = g_main_context_new();
    WebStreamer::main_loop = g_main_loop_new(WebStreamer::main_context, FALSE);
    g_main_context_push_thread_default(WebStreamer::main_context);

    GAsyncQueue* queue = (GAsyncQueue*)promise->user_data;

    bool initialized = this->Prepare(promise);
    if (!initialized)
    {
        g_main_context_pop_thread_default(WebStreamer::main_context);
        g_async_queue_push(queue, g_strdup("failed"));
        g_main_loop_unref(WebStreamer::main_loop);
        gst_deinit();
        WebStreamer::main_loop = NULL;
        WebStreamer::main_context = NULL;
        delete promise;
        return;
    }

    delete promise;
    promise = NULL;

    GSource* idle_source = g_idle_source_new();
    g_source_set_callback(idle_source,
        (GSourceFunc)main_loop_init, queue, NULL);
    g_source_set_priority(idle_source, G_PRIORITY_DEFAULT);
    g_source_attach(idle_source, WebStreamer::main_context);

    g_main_loop_run(WebStreamer::main_loop);


    this->Cleanup();
    g_main_context_pop_thread_default(WebStreamer::main_context);

    g_main_loop_unref(WebStreamer::main_loop);

    gst_deinit();
    WebStreamer::main_loop = NULL;
    WebStreamer::main_context = NULL;
    this->terminate_promise_->resolve();
    delete this->terminate_promise_;
    this->terminate_promise_ = NULL;
    delete this;
    set_webstreamer_instance(NULL);
}

WebStreamer::WebStreamer(plugin_interface_t* iface)
    : rtsp_session_pool_(NULL)
    , pool_clean_id_(0)
    , iface_(iface)
    , state_(State::IDLE)
{
    for (int i = 0; i < RTSPServer::SIZE; i++)
    {
        rtspserver_[i] = nullptr;
    }
}

void WebStreamer::Initialize(Promise* promise) {
    promise->SetWebStreamer(this);
    state_ = State::INITIALIZING;
    gst_init(NULL, NULL);
    GAsyncQueue* queue = g_async_queue_new();

    promise->user_data = queue;

    _main_thread = g_thread_new("webstreamer_main_loop",
        (GThreadFunc)MainloopEntry, promise);

    char* p = (char*)g_async_queue_pop(queue);
}

bool WebStreamer::Prepare(Promise* promise)
{
    // init RTSP Server
    std::string err = InitRTSPServer(&promise->data());
    if (!err.empty()) {
        promise->reject(err);
        return false;
    }
    state_ = State::RUNNING;
    promise->resolve();
    return true;
}


// FIXME(Mingyi): terminate, thread safe, state check and process
void WebStreamer::Terminate(Promise* promise)
{
    promise->SetWebStreamer(this);
    state_ = State::TERMINATING;
    this->terminate_promise_ = promise;
    g_main_loop_quit(this->main_loop);
}

bool WebStreamer::Cleanup()
{
    this->DestroyRTSPServer();
    return true;
}

gboolean WebStreamer::OnPromise(gpointer user_data)
{
    Promise* promise = (Promise*)user_data;
    WebStreamer* This = promise->webstreamer();
    This->OnPromise(promise);
    return G_SOURCE_REMOVE;
}

void WebStreamer::Call(Promise* promise)
{
    GSource *source;

    source = g_idle_source_new();

    promise->SetWebStreamer(this);
    g_source_set_callback(source, WebStreamer::OnPromise, promise, NULL);
    g_source_set_priority(source, G_PRIORITY_DEFAULT);
    g_source_attach(source, WebStreamer::main_context);
}

void WebStreamer::OnPromise(Promise *promise)
{
    const json& j = promise->meta();

    std::string action = j["action"];
    if (action == "create") {
        CreateApp(promise);
    } else if (action == "destroy") {
        DestroyApp(promise);
    } else {
        const std::string& name = j["name"];
        const std::string& type = j["type"];
        IApp* app = GetApp(name, type);
        if (!app) {
            promise->reject("processor not exists.");
            return;
        }
        app->On(promise);
        return;
    }
}


void WebStreamer::CreateApp(Promise* promise)
{
    const json& j = promise->meta();
    std::string name = j["name"];
    std::string type = j["type"];
    std::string uname = name + "@" + type;

    IApp* app = GetApp(name, type);
    if (app) {
        promise->reject(uname + " was an exist processor.");
        return;
    }

    app = Factory::Instantiate(type, name, this);
    if (!app) {
        promise->reject(type + " : type not supported");
        return;
    }


    if (app->Initialize(promise)) {
        apps_[uname] = app;
    } else {
        delete app;
        promise->reject("app initialize failed.");
        return;
    }
}
void WebStreamer::DestroyApp(Promise* promise) {
    const json& j = promise->meta();
    std::string name = j["name"];
    std::string type = j["type"];
    std::string uname = name + "@" + type;

    IApp* app = GetApp(name, type);
    if (!app)
    {
        promise->reject(uname + ": destrying a not exist app.");
        return;
    }

    if (!app->Destroy(promise)) {
        GST_INFO("%s is destrying.", uname.c_str());
        return;
    }
    delete app;
    apps_.erase(uname);

    promise->resolve();
    delete promise;
}

static gboolean pool_cleanup(GstRTSPSessionPool* *pool)
{
    if (*pool)
    {
        gst_rtsp_session_pool_cleanup(*pool);
        return G_SOURCE_CONTINUE;
    }
    return G_SOURCE_REMOVE;
}

std::string WebStreamer::InitRTSPServer(const nlohmann::json* option)
{
    std::string error;
    // not start rtsp server
    if (!option || option->find("rtsp_server") == option->cend())
    {
        return "";
    }
    const nlohmann::json& opt = *option;
    auto rtsp_server = opt["rtsp_server"];

    gint max_sessions = rtsp_server["max_sessions"];
    rtsp_session_pool_ = gst_rtsp_session_pool_new();
    gst_rtsp_session_pool_set_max_sessions(rtsp_session_pool_, max_sessions);


    gint port = 554;
    json::const_iterator it = rtsp_server.find("port");
    if (it != rtsp_server.cend())
    {
        port = rtsp_server["port"];
        RTSPServer* server = new RTSPServer(RTSPServer::RFC7826, port);
        if (!server)
        {
            error = "create RTSP Server failed ";
            goto _failed;
        }

        if (!server->Initialize(rtsp_session_pool_, NULL))
        {
            error = "initialize RTSP Server failed ";
            goto _failed;
        }
        rtspserver_[RTSPServer::RFC7826] = server;
    }

    it = rtsp_server.find("onvif_port");
    if (it != rtsp_server.cend())
    {
        port = rtsp_server["onvif_port"];
        RTSPServer* server = new RTSPServer(RTSPServer::ONVIF, port);
        if (!server)
        {
            error = "create Onvif RTSP Server failed ";
            goto _failed;
        }

        if (!server->Initialize(rtsp_session_pool_, NULL))
        {
            error = "initialize Onvif RTSP Server failed ";
            goto _failed;
        }
        rtspserver_[RTSPServer::ONVIF] = server;
    }


    pool_clean_id_ = g_timeout_add_seconds(10,
        (GSourceFunc)pool_cleanup, &rtsp_session_pool_);

    return "";
_failed:
    for (int i = 0; i < RTSPServer::SIZE; i++)
    {
        if (rtspserver_[i])
        {
            rtspserver_[i]->Destroy();
            delete rtspserver_[i];
            rtspserver_[i] = nullptr;
        }
    }

    if (rtsp_session_pool_)
    {
        g_object_unref(rtsp_session_pool_);
        rtsp_session_pool_ = NULL;
    }
    return error;
}

void WebStreamer::DestroyRTSPServer()
{
    for (int i = 0; i < RTSPServer::SIZE; i++)
    {
        if (rtspserver_[i])
        {
            rtspserver_[i]->Destroy();
            delete rtspserver_[i];
            rtspserver_[i] = nullptr;
        }
    }

    if (rtsp_session_pool_)
    {
        g_object_unref(rtsp_session_pool_);
        rtsp_session_pool_ = NULL;
    }
}
void WebStreamer::Notify(plugin_buffer_t* data, plugin_buffer_t* meta)
{
    if (!this->iface_ || !this->iface_->notify || (!data && !meta))
    {
        return;
    }

    iface_->notify(iface_, data, meta);
}

