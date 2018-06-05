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

#include "webrtctestclient.h"

using json = nlohmann::json;

GST_DEBUG_CATEGORY_STATIC(my_category);
#define GST_CAT_DEFAULT my_category

bool WebRTCTestClient::Initialize(Promise *promise)
{
    GST_DEBUG_CATEGORY_INIT(my_category, "webstreamer", 2, "libWebStreamer");
    webrtc_ep_ = new WebRTC(this, "test");
    return true;
}

bool WebRTCTestClient::Destroy(Promise *promise)
{
    if (webrtc_ep_) {
        delete webrtc_ep_;
        webrtc_ep_ = NULL;
    }
    return true;
}

void WebRTCTestClient::On(Promise *promise)
{
    const json &j = promise->meta();
    std::string action = j["action"];
    if (action == "startup") {
        Startup(promise);
    } else if (action == "stop") {
        Stop(promise);
    } else if (action == "remote_sdp") {
        set_remote_description(promise);
    } else if (action == "remote_candidate") {
        set_remote_candidate(promise);
    }
}
static gboolean message_handler(GstBus *bus,
                                GstMessage *message,
                                gpointer data)
{
    ElementWatcher *This = static_cast<ElementWatcher *>(data);
    This->OnMessage(bus, message);

    return TRUE;
}
void WebRTCTestClient::Startup(Promise *promise)
{
    if (webrtc_ep_) {
        const json &j = promise->data();
        std::string launch;
        if (j.find("launch") != j.end()) {
            launch = j["launch"];
        } else {
            promise->reject("webrtc test client startup failed! No launch!");
            return;
        }
        // printf("launch: %s\n", launch.c_str());
        webrtc_ep_->launch() = launch;
        if (webrtc_ep_->initialize(promise)) {
            GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(webrtc_ep_->pipeline()));
            gst_bus_add_watch(bus, message_handler, this);
            gst_object_unref(bus);

            GstElement *sink = gst_bin_get_by_name(GST_BIN(webrtc_ep_->pipeline()), "sink");
            set_sink(sink);
            if (sink == NULL) {
                GST_INFO("[webrtc test client] use multifilesink for test.");
            } else {
                GST_INFO("[webrtc test client] use raw image data for test.");
                GstPad *pad = gst_element_get_static_pad(sink, "sink");
                gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, ElementWatcher::on_have_data, this, NULL);
                gst_object_unref(pad);
            }

            promise->resolve();
            return;
        }
    }
    promise->reject("webrtc test client startup failed!");
}


void WebRTCTestClient::Stop(Promise *promise)
{
    if (webrtc_ep_) {
        webrtc_ep_->terminate();
        promise->resolve();
        return;
    }
    promise->reject("webrtc test client stop failed!");
}

void WebRTCTestClient::set_remote_description(Promise *promise)
{
    if (webrtc_ep_) {
        webrtc_ep_->set_remote_description(promise);
        promise->resolve();
        return;
    }
    promise->reject("webrtc test client set_remote_description failed!");
}
void WebRTCTestClient::set_remote_candidate(Promise *promise)
{
    if (webrtc_ep_) {
        webrtc_ep_->set_remote_candidate(promise);
        promise->resolve();
        return;
    }
    promise->reject("webrtc test client set_remote_candidate failed!");
}
