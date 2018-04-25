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

#ifndef _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVICE_H_
#define _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVICE_H_

#include <framework/endpoint.h>
#include <framework/rtspserver.h>
#include <utils/pipejoint.h>
#include <mutex>  // NOLINT


class IRTSPService : public IEndpoint
{
 public:
    IRTSPService(IApp *app, const std::string &name, RTSPServer::Type type);
    ~IRTSPService();

    virtual bool initialize(Promise *promise);
    virtual void terminate();

    bool Launch(const std::string &path,
                const std::string &launch,
                GCallback media_constructed,
                GCallback media_configure);
    bool Stop();

    std::string &path() { return path_; }
    const std::string &path() const { return path_; }

    std::string &launch() { return launch_; }
    const std::string &launch() const { return launch_; }

    static void on_rtsp_media_constructed(GstRTSPMediaFactory *factory,
                                          GstRTSPMedia *media,
                                          gpointer user_data);


 private:
    static void on_client_connected(GstRTSPServer *gstrtspserver,
                                    GstRTSPClient *client,
                                    gpointer user_data);
    static void on_new_session(GstRTSPClient *gstrtspclient,
                               GstRTSPSession *session,
                               gpointer user_data);
    static void onclosed(GstRTSPClient *client,
                         gpointer user_data);

    GstRTSPMediaFactory *factory_;
    RTSPServer *server_;
    std::string path_;
    std::string launch_;
    std::map<GstRTSPSession *, GstRTSPClient *> clients_;
    static std::mutex client_mutex_;


    PipeJoint video_joint_;
    PipeJoint audio_joint_;
};



#endif
