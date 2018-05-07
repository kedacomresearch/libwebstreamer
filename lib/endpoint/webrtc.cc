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

#include "webrtc.h"
#include <utils/typedef.h>
#include <gst/sdp/sdp.h>
#include <gst/webrtc/webrtc.h>

using json = nlohmann::json;

GST_DEBUG_CATEGORY_STATIC(my_category);
#define GST_CAT_DEFAULT my_category

WebRTC::WebRTC(IApp *app, const std::string &name)
    : IEndpoint(app, name)
    , pipeline_(NULL)
    , bin_(NULL)
    , webrtc_(NULL)
{
}

WebRTC::~WebRTC()
{
}
// void WebRTC::on_answer_created(GstPromise *promise, gpointer user_data)
// {
//     WebRTC *webrtc = static_cast<WebRTC *>(user_data);
//     GstWebRTCSessionDescription *answer = NULL;
//     // g_assert(gst_promise_wait(promise) == GST_PROMISE_RESULT_REPLIED);
//     const GstStructure *reply = gst_promise_get_reply(promise);
//     gst_structure_get(reply, "answer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &answer, NULL);
//     gst_promise_unref(promise);

//     g_signal_emit_by_name(webrtc->webrtc_, "set-local-description", answer, NULL);

//     /* Send answer to peer */
//     std::string sdp_answer(gst_sdp_message_as_text(answer->sdp));
//     json data;
//     data["type"] = "answer";
//     data["sdp"] = sdp_answer;
//     json meta;
//     meta["topic"] = "webrtc";
//     meta["origin"] = webrtc->app()->uname();
//     meta["type"] = "sdp";

//     GST_DEBUG("[webrtc] %p local description created (answer).", webrtc->webrtc_);

//     webrtc->app()->Notify(data, meta);

//     gst_webrtc_session_description_free(answer);
// }
void WebRTC::on_ice_candidate(GstElement *webrtc_element G_GNUC_UNUSED,
                              guint mlineindex,
                              gchar *candidate,
                              gpointer user_data G_GNUC_UNUSED)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    json data;
    data["candidate"] = candidate;
    data["sdpMLineIndex"] = mlineindex;

    std::string ice_candidate = data.dump();

    // printf("\nice:%s\n", ice_candidate.c_str());

    json meta;
    meta["topic"] = "webrtc";
    meta["origin"] = webrtc->app()->uname();
    meta["type"] = "ice";

    GST_DEBUG("[webrtc] %p (%s) local candidate created.", webrtc->webrtc_, webrtc->role_.c_str());

    webrtc->app()->Notify(data, meta);
}
void WebRTC::on_sdp_created(GstPromise *promise, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstWebRTCSessionDescription *sdp = NULL;
    const GstStructure *reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, webrtc->role_.c_str(), GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &sdp, NULL);
    gst_promise_unref(promise);
    // gst_sdp_media_add_attribute((GstSDPMedia *)&g_array_index(offer->sdp->medias, GstSDPMedia, 0),
    //                             "fmtp",
    //                             "96 profile-level-id=42e01f");


    /* Send sdp to peer */
    std::string sdp_offer(gst_sdp_message_as_text(sdp->sdp));
    json data;
    data["type"] = webrtc->role_;
    data["sdp"] = sdp_offer;
    json meta;
    meta["topic"] = "webrtc";
    meta["origin"] = webrtc->app()->uname();
    meta["type"] = "sdp";

    g_signal_emit_by_name(webrtc->webrtc_, "set-local-description", sdp, NULL);
    GST_DEBUG("[webrtc] %p (%s) local description created.", webrtc->webrtc_, webrtc->role_.c_str());

    webrtc->app()->Notify(data, meta);

    gst_webrtc_session_description_free(sdp);
}
void WebRTC::on_negotiation_needed(GstElement *element, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstPromise *promise = gst_promise_new_with_change_func(on_sdp_created, user_data, NULL);
    g_signal_emit_by_name(webrtc->webrtc_, "create-offer", NULL, promise);
}
static GstPadProbeReturn cb_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
    printf("-");
    return GST_PAD_PROBE_OK;
}
void WebRTC::on_webrtc_pad_added(GstElement *webrtc_element, GstPad *new_pad, gpointer user_data)
{
    printf("=========on_webrtc_pad_added===========\n");
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstElement *video_payloader = gst_bin_get_by_name(GST_BIN(webrtc->pipeline_), "video_payloader");
    GstPad *pad = gst_element_get_static_pad(video_payloader, "sink");
    gst_pad_link(new_pad, pad);
    gst_object_unref(pad);
}

bool WebRTC::initialize(Promise *promise)
{
    GST_DEBUG_CATEGORY_INIT(my_category, "webstreamer", 2, "libWebStreamer");
    IEndpoint::protocol() = "webrtc";

    GError *error = NULL;

    std::string launch = "webrtcbin name=webrtc ";
    if (!app()->video_encoding().empty()) {
        std::string video_enc = app()->video_encoding();
        // launch += "rtspsrc location=rtsp://172.16.66.65/id=1 ! rtph264depay ! queue ! ";
        launch += "rtp" + video_enc + "pay name=pay0 ! queue ! " +
                  "application/x-rtp,media=video,encoding-name=" + uppercase(video_enc) +
                  ",payload=96 ! webrtc. ";
    }
    if (!app()->audio_encoding().empty()) {
        std::string audio_enc = app()->audio_encoding();
        launch += "rtp" + audio_enc + "pay name=pay1 ! queue ! " +
                  "application/x-rtp,media=audio,encoding-name=" + uppercase(audio_enc);
        if (uppercase(audio_enc) == "PCMA") {
            launch += ",clock-rate=8000";
        }
        launch += ",payload=97,name=audio_cap ! webrtc. ";
    }
    const json &j = promise->data();
    if (j.find("launch") != j.end()) {
        launch = j["launch"];
    }
    // printf("===============>  %s\n", launch.c_str());
    bin_ = gst_parse_launch(launch.c_str(), &error);

    if (error) {
        GST_ERROR("[webrtc] Failed to parse launch: %s", error->message);
        g_error_free(error);
        g_clear_object(&bin_);
        bin_ = NULL;
        return false;
    }
    pipeline_ = gst_pipeline_new(NULL);
    gst_bin_add(GST_BIN(pipeline_), bin_);
    g_assert_nonnull(pipeline_);
    webrtc_ = gst_bin_get_by_name(GST_BIN(pipeline_), "webrtc");

    // specific parameter
    if (app()->video_encoding() == "h264") {
        GstElement *payloader = gst_bin_get_by_name(GST_BIN(pipeline_), "pay0");
        g_object_set(G_OBJECT(payloader), "config-interval", -1, NULL);
        gst_object_unref(payloader);
    }

    g_signal_connect(webrtc_, "on-ice-candidate", G_CALLBACK(WebRTC::on_ice_candidate), this);
    if (j.find("launch") == j.end()) {
        role_ = "offer";
        GstPromise *promise = gst_promise_new_with_change_func(WebRTC::on_sdp_created, this, NULL);
        g_signal_emit_by_name(webrtc_, "create-offer", NULL, promise);
        // g_signal_connect(webrtc_, "on-negotiation-needed", G_CALLBACK(WebRTC::on_negotiation_needed), this);
    } else {
        role_ = "answer";
        // GstElement *test_element = gst_bin_get_by_name(GST_BIN(pipeline_), "video_payloader");
        // GstPad *pad = gst_element_get_static_pad(test_element, "sink");
        // gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, cb_have_data, this, NULL);
        // gst_object_unref(pad);
        // g_signal_connect(webrtc_, "pad-added", G_CALLBACK(WebRTC::on_webrtc_pad_added), this);
    }

    static int session_count = 0;
    if (!app()->video_encoding().empty()) {
        GST_DEBUG("[webrtc] %p media constructed: video", webrtc_);

        static std::string media_type = "video";
        std::string pipejoint_name = std::string("webrtc_video_endpoint_joint_") +
                                     name() +
                                     std::to_string(session_count);
        video_joint_ = make_pipe_joint(media_type, pipejoint_name);

        app()->add_pipe_joint(video_joint_.upstream_joint);

        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), video_joint_.downstream_joint));

        GstElement *video_pay = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline_), "pay0");
        g_warn_if_fail(gst_element_link(video_joint_.downstream_joint, video_pay));
    }
    if (!app()->audio_encoding().empty()) {
        GST_DEBUG("[webrtc] %p media constructed: audio", webrtc_);

        static std::string media_type = "audio";
        std::string pipejoint_name = std::string("webrtc_audio_endpoint_joint_") +
                                     name() +
                                     std::to_string(session_count);
        audio_joint_ = make_pipe_joint(media_type, pipejoint_name);

        app()->add_pipe_joint(audio_joint_.upstream_joint);

        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), audio_joint_.downstream_joint));

        GstElement *audio_pay = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline_), "pay1");
        if (!gst_element_link(audio_joint_.downstream_joint, audio_pay)) {
            GST_ERROR("[webrtc] %p (%s) audio joint pad link failed.", webrtc_, role_.c_str());
        }
    }
    session_count++;


    GstStateChangeReturn ret = gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        GST_DEBUG("[webrtc] %p initialize failed.", webrtc_);
    }
    GST_DEBUG("[webrtc] %p (%s) initialize done.", webrtc_, role_.c_str());

    return true;
}

void WebRTC::terminate()
{
    // dynamicly unlink
    if (!app()->video_encoding().empty() &&
        video_joint_.upstream_joint != NULL) {
        app()->remove_pipe_joint(video_joint_.upstream_joint);
    }
    if (!app()->audio_encoding().empty() &&
        audio_joint_.upstream_joint != NULL) {
        app()->remove_pipe_joint(audio_joint_.upstream_joint);
    }
    if (pipeline_) {
        gst_element_set_state(GST_ELEMENT(pipeline_), GST_STATE_NULL);
        gst_bin_remove(GST_BIN(pipeline_), bin_);
        gst_object_unref(pipeline_);
    }
    if (webrtc_) {
        gst_object_unref(webrtc_);
        webrtc_ = NULL;
    }
    GST_DEBUG("[webrtc] %p (%s) terminate done.", webrtc_, role_.c_str());
}

void WebRTC::set_remote_description(Promise *promise)
{
    const json &j = promise->data();
    std::string sdp_info = j["sdp"];
    std::string type = j["type"];
    // printf("\n%s\n", sdp_info.c_str());

    GstWebRTCSessionDescription *sdp;
    GstSDPMessage *sdp_msg;
    gst_sdp_message_new(&sdp_msg);
    gst_sdp_message_parse_buffer((guint8 *)sdp_info.c_str(), (guint)sdp_info.size(), sdp_msg);
    sdp = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_ANSWER, sdp_msg);
    if (type == "offer")
        sdp->type = GST_WEBRTC_SDP_TYPE_OFFER;
    else
        sdp->type = GST_WEBRTC_SDP_TYPE_ANSWER;
    g_signal_emit_by_name(webrtc_, "set-remote-description", sdp, NULL);
    GST_DEBUG("[webrtc] %p (%s) set remote description.", webrtc_, role_.c_str());

    if (role_ == "answer") {
        GstPromise *promise = gst_promise_new_with_change_func(WebRTC::on_sdp_created, this, NULL);
        g_signal_emit_by_name(webrtc_, "create-answer", NULL, promise);
    }
}
void WebRTC::set_remote_candidate(Promise *promise)
{
    const json &j = promise->data();
    std::string candidate = j["candidate"];
    int sdpmlineindex = j["sdpMLineIndex"];
    g_signal_emit_by_name(webrtc_, "add-ice-candidate", sdpmlineindex, candidate.c_str());
    GST_DEBUG("[webrtc] %p (%s) set remote candidate.", webrtc_, role_.c_str());
}
