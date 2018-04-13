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

#include "rtspserver.h"
#include <gst/rtsp-server/rtsp-onvif-server.h>
#include <webstreamer.h>
RTSPServer::RTSPServer(Type type, int port)
	: server_(NULL)
	, type_(type)
	, port_(port)
{
}

RTSPServer::~RTSPServer()
{
}

bool RTSPServer::Initialize(GstRTSPSessionPool* pool, int port )
{
	// FIXME: onvif new link error
	// server_ = ( type_ == RFC7826 ?
    // gst_rtsp_server_new() : gst_rtsp_onvif_server_new() );

	server_ = gst_rtsp_server_new();
	if (pool) {
		gst_rtsp_server_set_session_pool(server_, pool);
	}

	gchar* service = g_strdup_printf("%d", port);
	gst_rtsp_server_set_service(server_, service);
	g_free(service);
	return true;
}
bool RTSPServer::Initialize(GstRTSPSessionPool* pool,
    GMainContext* context /*= NULL*/)
{
	// FIXME: onvif new link error
	// server_ = ( type_ == RFC7826 ? gst_rtsp_server_new()
    // : gst_rtsp_onvif_server_new() );

	server_ = gst_rtsp_server_new();
	if (pool) {
		gst_rtsp_server_set_session_pool(server_, pool);
	}

	gchar* service = g_strdup_printf("%d", port_);
	gst_rtsp_server_set_service(server_, service);
	g_free(service);

	gst_rtsp_server_attach(server_, context);

	return true;
}

void RTSPServer::Destroy()
{
	if (server_)
	{
		g_object_unref(server_);
		server_ = NULL;
	}
}
