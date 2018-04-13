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

#ifndef _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVERVER_H___
#define _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVERVER_H___


#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

class RTSPServer
{
 public:
	enum Type
	{
		RFC7826 = 0,  // RTSP 2.0 RFC7826
		ONVIF,
		UNDEFINED,
		SIZE = UNDEFINED,
		RTSP20 = RFC7826
	};
 public:
    explicit RTSPServer(Type type, int port = -1);
	~RTSPServer();
	bool Initialize(GstRTSPSessionPool* pool, int port);  // TO BE DEL
	bool Initialize(GstRTSPSessionPool* pool, GMainContext* context = NULL);
	void Destroy();
	Type type() { return type_; }
	int port() { return port_; }
	GstRTSPServer* server() { return server_; }
 protected:
	GstRTSPServer* server_;
	Type           type_;
	int            port_;
};


#endif  // _LIBWEBSTREAMER_ENDPOINT_RTSP_SERVERVER_H___
