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

#include "elementwatcher.h"
#include <webstreamer.h>
#include <gst/video/video.h>

using json = nlohmann::json;

GST_DEBUG_CATEGORY_STATIC(my_category);
#define GST_CAT_DEFAULT my_category
/* receive spectral data from element message */
static gboolean
message_handler(GstBus *bus, GstMessage *message, gpointer data)
{
    ElementWatcher *This = static_cast<ElementWatcher *>(data);
    This->OnMessage(bus, message);

    return TRUE;
}

bool ElementWatcher::Initialize(Promise *promise)
{
    GST_DEBUG_CATEGORY_INIT(my_category, "webstreamer", 2, "libWebStreamer");
    // promise->resolve();
    return true;
}

void ElementWatcher::On(Promise *promise)
{
    const json &j = promise->meta();
    std::string action = j["action"];
    if (action == "startup") {
        Startup(promise);
    } else if (action == "stop") {
        Stop(promise);
    }
}

gboolean ElementWatcher::on_save_image(gpointer user_data)
{
    ElementWatcher *app = static_cast<ElementWatcher *>(user_data);

    GstSample *from_sample;
    g_object_get(app->sink_, "last-sample", &from_sample, NULL);
    if (from_sample == NULL) {
        GST_ERROR("[element watcher] failed getting sample.");
        return FALSE;
    }
    GstCaps *caps = gst_caps_from_string("image/bmp");

    if (caps == NULL) {
        GST_ERROR("[element watcher] failed getting caps.");
        return FALSE;
    }

    GError *err = NULL;
    GstSample *to_sample = gst_video_convert_sample(from_sample, caps, GST_CLOCK_TIME_NONE, &err);
    gst_caps_unref(caps);
    gst_sample_unref(from_sample);

    if (to_sample == NULL && err) {
        GST_ERROR("[element watcher] failed converting frame.");
        GST_ERROR("[element watcher] error : %s", err->message);
        return FALSE;
    }
    GstBuffer *buf = gst_sample_get_buffer(to_sample);

    GstClockTime stream_time = GST_BUFFER_TIMESTAMP(buf);
    GstClockTime duration = GST_BUFFER_DURATION(buf);

    GstMapInfo map_info;
    if (!gst_buffer_map(buf, &map_info, GST_MAP_READ)) {
        GST_ERROR("[element watcher] could not get image data from gstbuffer");
        gst_sample_unref(to_sample);

        return FALSE;
    }

    gst_sample_unref(to_sample);

    json data;
    data["duration"] = duration;
    data["stream-time"] = stream_time;

    unsigned char *bmp_buffer = new unsigned char[map_info.size];
    memcpy(bmp_buffer, map_info.data, map_info.size);
    app->bmps_.push_back(bmp_buffer);
    uint64_t pointer = reinterpret_cast<uint64_t>(bmp_buffer);
    // printf("%p\n%ld\n%p\n",bmp_buffer,pointer,reinterpret_cast<unsigned char *>(pointer));

    data["data"] = pointer;
    data["size"] = map_info.size;

    json meta;
    meta["topic"] = "image_data";
    meta["origin"] = app->uname();

    app->Notify(data, meta);
    return TRUE;
}

GstPadProbeReturn ElementWatcher::on_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
    ElementWatcher *app = static_cast<ElementWatcher *>(user_data);
    if (app->cur_frame_ <= (app->frame_ * 2)) {
        if (++app->cur_frame_ % 2 == 0)
            app->on_save_image(user_data);
        return GST_PAD_PROBE_OK;
    } else {
        return GST_PAD_PROBE_REMOVE;
    }
}
void ElementWatcher::Startup(Promise *promise)
{
    GError *error = NULL;

    const json &j = promise->data();
    const std::string &launch = j["launch"];

    /* Build the pipeline */
    const char *name = this->uname().c_str();
    pipeline_ = gst_pipeline_new(name);
    GstElement *bin = gst_parse_launch(launch.c_str(), &error);
    gst_bin_add(GST_BIN(pipeline_), bin);

    gst_element_set_state(pipeline_, GST_STATE_PLAYING);

    GstBus *bus = gst_element_get_bus(pipeline_);
    gst_bus_add_watch(bus, message_handler, this);
    gst_object_unref(bus);

    sink_ = gst_bin_get_by_name(GST_BIN(pipeline_), "sink");
    if (sink_ == NULL) {
        GST_INFO("[element watcher] use multifilesink for test.");
    } else {
        GST_INFO("[element watcher] use raw image data for test.");
        frame_ = j["frame"];
        GstPad *pad = gst_element_get_static_pad(sink_, "sink");
        gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, ElementWatcher::on_have_data, this, NULL);
        gst_object_unref(pad);
    }
    promise->resolve();
}

void ElementWatcher::Stop(Promise *promise)
{
    gst_element_set_state(pipeline_, GST_STATE_NULL);
    if (time_id_ != -1) {
        g_source_remove(time_id_);
    }
    for (auto it : bmps_) {
        delete[] it;
    }
    promise->resolve();
}

bool ElementWatcher::Destroy(Promise *promise)
{
    // IApp::Destroy(promise);
    return true;
}
void ElementWatcher::OnMessage(GstBus *bus, GstMessage *message)
{
    if (message->type == GST_MESSAGE_ELEMENT) {
        const gchar *name = GST_MESSAGE_SRC_NAME(message);
        const GstStructure *s = gst_message_get_structure(message);
        const gchar *type = gst_structure_get_name(s);

        if (!g_strcmp0(type, "spectrum")) {
            OnSpectrum(name, s);
        } else if (!g_strcmp0(type, "GstMultiFileSink")) {
            OnMultifilesink(name, s);
        }
    }
}

void ElementWatcher::OnSpectrum(const std::string &name,
                                const GstStructure *s)
{
    GstClockTime endtime;
    const GValue *magnitudes;
    const GValue *phases;
    const GValue *mag, *phase;
    guint size;

    if (!gst_structure_get_clock_time(s, "endtime", &endtime))
        endtime = GST_CLOCK_TIME_NONE;

    magnitudes = gst_structure_get_value(s, "magnitude");
    phases = gst_structure_get_value(s, "phase");
    size = gst_value_list_get_size(magnitudes);
    json data;


    json::array_t mags;
    for (guint i = 0; i < size; ++i) {
        mag = gst_value_list_get_value(magnitudes, i);
        phase = gst_value_list_get_value(phases, i);
        mags.push_back(g_value_get_float(mag));
    }
    data["name"] = name;
    data["endtime"] = endtime;
    data["magnitude"] = mags;

    json meta;
    meta["topic"] = "spectrum";
    meta["origin"] = this->uname();

    Notify(data, meta);
}

void ElementWatcher::OnMultifilesink(const std::string &name,
                                     const GstStructure *s)
{
    GstClockTime duration, stream_time;
    const GValue *val;

    if (!gst_structure_get_clock_time(s, "duration", &duration))
        duration = GST_CLOCK_TIME_NONE;

    if (!gst_structure_get_clock_time(s, "stream-time", &stream_time))
        stream_time = GST_CLOCK_TIME_NONE;

    val = gst_structure_get_value(s, "filename");
    const gchar *filename = g_value_get_string(val);

    json data;
    data["name"] = name;
    data["duration"] = duration;
    data["stream-time"] = stream_time;

    data["filename"] = filename;

    json meta;
    meta["topic"] = "multifilesink";
    meta["origin"] = this->uname();

    Notify(data, meta);
}
