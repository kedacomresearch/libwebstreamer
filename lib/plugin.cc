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

#include <iostream>
#include <exception>

#include "nlohmann/json.hpp"

#include <gst/gst.h>
#include "./plugin_interface.h"
#include "./webstreamer.h"

#define __VERSION__ "0.1.1"

static WebStreamer *_webstreamer = NULL;
WebStreamer *get_webstreamer_instance()
{
    return _webstreamer;
}
void set_webstreamer_instance(WebStreamer *inst)
{
    _webstreamer = inst;
}


static void init(const void *iface,
                 const void *context,
                 const plugin_buffer_t *data,
                 plugin_callback_fn callback)
{
    plugin_interface_t *self = (plugin_interface_t *)iface;

    if (_webstreamer) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err, const_error_msg("init", "webstreamer not permit to create multi instances."));
        callback(self, context, 1, &err);
        return;
    }

    nlohmann::json j;
    if (data) {
        try {
            j = nlohmann::json::parse(std::string(
                (const char *)data->data, data->size));
        } catch (std::exception &) {
            plugin_buffer_t err;
            plugin_buffer_string_set(&err, const_error_msg("init", "invalid option format(should be json)."));
            callback(self, context, 1, &err);
            return;
        }
    }
    _webstreamer = new WebStreamer(self);
    Promise *promise = new Promise((void *)self,
                                   context,
                                   callback,
                                   nlohmann::json(),
                                   j);
    _webstreamer->Initialize(promise);
    return;
}


static void call(const void *iface,
                 const void *context,
                 plugin_buffer_t *data,
                 plugin_buffer_t *meta,
                 plugin_callback_fn callback)
{
    plugin_interface_t *self = (plugin_interface_t *)iface;
    if (!_webstreamer) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err, const_error_msg("call", "webstreamer not constructed."));
        callback(self, context, 1, &err);
        return;
    }

    if (WebStreamer::State::RUNNING != _webstreamer->state()) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err, const_error_msg("call", "trying call method befor running state.."));
        callback(self, context, 1, &err);
        return;
    }

    if (!meta || !meta->data || !meta->size) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err,
                                 const_error_msg("call", "action not specified."));
        callback(self, context, 1, &err);
        return;
    }
    nlohmann::json jmeta;
    try {
        const char *begin = (const char *)meta->data;
        const char *end = begin + meta->size;

        jmeta = nlohmann::json::parse(begin, end);
    } catch (std::exception &) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err,
                                 const_error_msg("call", "invalid meta json string."));
        callback(self, context, 1, &err);
        return;
    }

    nlohmann::json jdata;
    try {
        if (data && data->data && data->size) {
            const char *begin = (const char *)data->data;
            const char *end = begin + data->size;
            jdata = nlohmann::json::parse(begin, end);
        }
    } catch (std::exception &) {
        plugin_buffer_t err;
        plugin_buffer_string_set(&err,
                                 const_error_msg("call", "invalid data json string."));
        callback(self, context, 1, &err);
        return;
    }

    Promise *promise = new Promise((void *)self,
                                   context,
                                   callback,
                                   jmeta,
                                   jdata);
    _webstreamer->Call(promise);
    promise = nullptr;
}

static void terminate(const void *iface,
                      const void *context,
                      plugin_callback_fn callback)
{
    plugin_interface_t *self = (plugin_interface_t *)iface;

    Promise *promise = new Promise((void *)self, context, callback);
    _webstreamer->Terminate(promise);
}

PLUGIN_INTERFACE(__VERSION__, init, call, terminate)
