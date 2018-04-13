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

#include <webstreamer.h>
#include "rtspservice.h"
#include <gst/rtsp-server/rtsp-onvif-server.h>
IRTSPService::IRTSPService(IApp* app,
    const std::string& name, RTSPServer::Type type)
    :IEndpoint(app, name),
    factory_(NULL)
{
    server_ = app->webstreamer().GetRTSPServer(type);
}

IRTSPService::~IRTSPService()
{
}


static  void Notify(gpointer      data, GObject      *where_the_object_was)
{
    g_print(" data : %x\n", data);
    g_print(" where_the_object_was : %x\n", where_the_object_was);
}
void IRTSPService::on_tear_down(GstRTSPClient *client,
    GstRTSPContext *ctx, gpointer user_data)
{
    IRTSPService *rtsp_service = static_cast<IRTSPService *>(user_data);
    auto it = std::find_if(rtsp_service->clients_.begin(),
        rtsp_service->clients_.end(),
                        [client](GstRTSPClient *curr_client) {
                            return (curr_client == client);
                        });
    if (it != rtsp_service->clients_.end())
    {
        rtsp_service->clients_.erase(it);
        GST_DEBUG("[rtsp-server] client: %p removed", client);
    }
}
void IRTSPService::on_client_connected(GstRTSPServer *gstrtspserver,
    GstRTSPClient *client, gpointer user_data)
{
    IRTSPService * rtsp_service = static_cast<IRTSPService *>(user_data);
    rtsp_service->clients_.push_back(client);
    g_signal_connect(client, "teardown-request",
        (GCallback)(rtsp_service->on_tear_down), user_data);
    GST_DEBUG("[rtsp-server] client: %p connected", client);
}
bool IRTSPService::Launch(const std::string& path, const std::string& launch,
    GCallback media_constructed, GCallback media_configure)
{
    GstRTSPServer* server = server_->server();
    GstRTSPMountPoints *mount_points =
        gst_rtsp_server_get_mount_points(server);

    factory_ = gst_rtsp_media_factory_new();
    // if you want multiple clients to see the same video,
    // set the shared property to TRUE
    gst_rtsp_media_factory_set_shared(factory_, TRUE);

    gst_rtsp_media_factory_set_launch(factory_, launch.c_str());
    if (media_constructed)
    {
        g_signal_connect(factory_, "media-constructed",
            (GCallback)media_constructed, (gpointer)(this));
    }

    if (media_configure)
    {
        g_signal_connect(factory_, "media-configure",
            (GCallback)media_configure, (gpointer)(this));
    }


    gst_rtsp_mount_points_add_factory(mount_points, path.c_str(), factory_);
    g_object_unref(mount_points);

    GST_DEBUG("[rtsp-server] %s launched to %s", name_.c_str(), path.c_str());
    path_ = path;
    g_object_weak_ref(G_OBJECT(factory_), Notify, factory_);

    g_signal_connect(server, "client-connected",
        (GCallback)on_client_connected, (gpointer)(this));

    return true;
}


bool IRTSPService::Stop()
{
    if (!clients_.empty())
    {
        for (auto client : clients_)
            gst_rtsp_client_close(client);
    }
    if (factory_)
    {
        GstRTSPServer* server = server_->server();
        GstRTSPMountPoints *mount_points =
            gst_rtsp_server_get_mount_points(server);

        gst_rtsp_mount_points_remove_factory(mount_points, path_.c_str());
        g_object_unref(mount_points);

        factory_ = NULL;
    }
    return true;
}
