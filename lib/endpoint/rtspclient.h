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

#ifndef _LIBWEBSTREAMER_ENDPOINT_RTSP_CLIENT_H_
#define _LIBWEBSTREAMER_ENDPOINT_RTSP_CLIENT_H_


#include <framework/app.h>

class RtspClient : public IEndpoint
{
 public:
    RtspClient(IApp *app, const std::string &name);
    ~RtspClient();

    virtual bool initialize(Promise *promise);
    virtual void terminate();

 private:
    bool add_to_pipeline();
    static void on_rtspsrc_pad_added(GstElement *src,
                                     GstPad *src_pad,
                                     gpointer depay);
    static gboolean on_rtspsrc_select_stream(GstElement *src,
                                             guint stream_id,
                                             GstCaps *stream_caps,
                                             gpointer rtspclient);
    // only for test
    static GstPadProbeReturn on_monitor_data(GstPad *pad,
                                             GstPadProbeInfo *info,
                                             gpointer rtspclient);

    GstElement *rtspsrc_;
    GstElement *rtpdepay_video_;
    GstElement *parse_video_;
    GstElement *rtpdepay_audio_;
    bool add_to_pipeline_;
};
#endif
