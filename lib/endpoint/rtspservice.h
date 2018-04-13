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

#include <string>
#include <list>

#include <framework/endpoint.h>
#include <framework/rtspserver.h>

class IRTSPService : public IEndpoint
{
 public:
    IRTSPService(IApp* app, const std::string& name, RTSPServer::Type type);
    ~IRTSPService();

    virtual bool Launch(const std::string& path, const std::string& launch,
                 GCallback media_constructed, GCallback media_configure);
    virtual bool Stop();
 protected:
    static void on_client_connected(GstRTSPServer *gstrtspserver,
        GstRTSPClient *client, gpointer user_data);
    static void on_tear_down(GstRTSPClient *client,
        GstRTSPContext *ctx, gpointer user_data);
 private:
    GstRTSPMediaFactory * factory_;
    RTSPServer*           server_;
    std::string           path_;
    std::list<GstRTSPClient *> clients_;
};



#endif
