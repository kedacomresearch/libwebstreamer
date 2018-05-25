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

#ifndef _LIBWEBSTREAMER_PROMISE_H_
#define _LIBWEBSTREAMER_PROMISE_H_

#include <vector>
#include <list>
#include <map>
#include <string>
#include <gst/gst.h>
#include <plugin_interface.h>
#include <nlohmann/json.hpp>

class WebStreamer;
class IApp;
class Promise
{
 public:
    Promise(void* iface, const void* context, plugin_callback_fn callback,
        const nlohmann::json& jmeta = nlohmann::json(),
        const nlohmann::json& jdata = nlohmann::json())
        : user_data(NULL)
        , iface_((plugin_interface_t *)iface)
        , context_(context)
        , jdata_(jdata)
        , jmeta_(jmeta)
        , responsed_(false)
        , webstreamer_(nullptr)
        , app_(nullptr)
        , callback_(callback)
    {
    }

    void resolve(const nlohmann::json& param) {
        if (responsed_) {
            return;  // response repated
        }

        const std::string& s = param.dump();
        plugin_buffer_t data;
        plugin_buffer_string_set(&data, s.c_str());
        callback_(iface_, context_, 1, &data);
    }

    void resolve() {
        if (responsed_) {
            return;  // response repated
        }

        callback_(iface_, context_, 0, NULL);
    }

    void reject(const std::string& message) {
        if (responsed_) {
            return;  // response repated
        }
        plugin_buffer_t data;
        plugin_buffer_string_set(&data, message.c_str());
        callback_(iface_, context_, 1, &data);
    }

    const nlohmann::json& data() const
    {
        return this->jdata_;
    }

    const nlohmann::json& meta() const
    {
        return this->jmeta_;
    }

    IApp*        app() { return app_;  }
    WebStreamer* webstreamer() {return webstreamer_;}
    void* user_data;
 protected:
    void SetWebStreamer(WebStreamer* ws) {
        webstreamer_ = ws;
    }
    void SetApp(IApp* app) {
        app_ = app;
    }
    friend class WebStreamer;

 private:
    plugin_interface_t*       iface_;
    const void*               context_;
    nlohmann::json            jdata_;
    nlohmann::json            jmeta_;
    bool                      responsed_;
    WebStreamer*              webstreamer_;
    IApp*                     app_;
    plugin_callback_fn        callback_;
};

#endif  // _LIBWEBSTREAMER_PROMISE_H_
