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

#include "app.h"
#include <webstreamer.h>

bool IApp::Initialize(Promise* promise)
{
    if (!pipeline_)
    {
        pipeline_ = gst_pipeline_new((const gchar*)uname().c_str());
    }

    if (pipeline_) {
        // promise->resolve();
        return true;
    } else {
        // promise->reject("create pipeline failed.");
        return false;
    }
}


bool IApp::Destroy(Promise* promise)
{
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_NULL);
        g_object_unref(pipeline_);
        pipeline_ = NULL;
    }
    webstreamer_ = NULL;
    return true;
}


void IApp::Notify(const nlohmann::json& data, const nlohmann::json& meta)
{
    const std::string& jdata = data.dump();
    const std::string& jmeta = meta.dump();
    plugin_buffer_t d;
    plugin_buffer_t m;
    plugin_buffer_string_set(&d, jdata.c_str());
    plugin_buffer_string_set(&m, jmeta.c_str());
    this->webstreamer_->Notify(&d, &m);
}
