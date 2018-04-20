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

#ifndef _LIBWEBSTREAMER_APPLICATION_LIVESTREAM_H_
#define _LIBWEBSTREAMER_APPLICATION_LIVESTREAM_H_

#include <framework/app.h>
#include <mutex>  // NOLINT

// #define USE_AUTO_SINK 1
struct sink_link
{
    GstPad *tee_pad;
    GstElement *upstream_joint;
    void *pipeline;
    gboolean video_probe_invoke_control;
    gboolean audio_probe_invoke_control;

    sink_link(GstPad *pad, GstElement *joint, void *pipe)
        : upstream_joint(joint)
        , pipeline(pipe)
        , tee_pad(pad)
        , video_probe_invoke_control(FALSE)
        , audio_probe_invoke_control(FALSE)
    {
    }
};
class WebStreamer;
class LiveStream : public IApp
{
 public:
    APP(LiveStream);

    LiveStream(const std::string &name, WebStreamer *ws);
    ~LiveStream();
    void add_pipe_joint(GstElement *upstream_joint);
    void remove_pipe_joint(GstElement *upstream_joint);

    virtual void On(Promise *promise);
    virtual bool Initialize(Promise *promise);
    virtual bool Destroy(Promise *promise);

 protected:
    void add_performer(Promise *promise);
    void add_audience(Promise *promise);
    void remove_audience(Promise *promise);
    void Startup(Promise *promise);
    void Stop(Promise *promise);

    bool on_add_endpoint(IEndpoint *endpoint);
    virtual bool MessageHandler(GstMessage *msg);

 private:
    static GstPadProbeReturn
	on_tee_pad_remove_video_probe(GstPad *pad,
		GstPadProbeInfo *probe_info, gpointer data);

    static GstPadProbeReturn
	on_tee_pad_remove_audio_probe(GstPad *pad,
		GstPadProbeInfo *probe_info, gpointer data);

    static GstPadProbeReturn on_monitor_data(GstPad *pad,
                                             GstPadProbeInfo *info,
                                             gpointer user_data);
    IEndpoint *find_audience(const std::string &name);
    GstElement *video_tee_;
    GstElement *audio_tee_;
    IEndpoint *performer_;
    std::list<IEndpoint *> audiences_;

    std::list<sink_link *> sinks_;  // all the request pad of tee,
	                                // release when removing from
	                                // pipeline

    GstPad *video_tee_pad_;
    GstElement *fake_video_queue_;
    GstElement *fake_video_sink_;
    GstPad *audio_tee_pad_;
    GstElement *fake_audio_queue_;
    GstElement *fake_audio_sink_;
#ifdef USE_AUTO_SINK
    GstElement *fake_video_decodec_;
    GstElement *fake_audio_decodec_;
    GstElement *fake_audio_convert_;
    GstElement *fake_audio_resample_;
#endif
    static std::mutex joint_mutex_;
};

#endif
