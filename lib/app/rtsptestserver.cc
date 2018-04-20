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

#include "rtsptestserver.h"
#include <webstreamer.h>
#include <endpoint/rtspservice.h>
using json = nlohmann::json;

class RTSPTestService : public IRTSPService
{
 public:
    RTSPTestService(IApp* app, const std::string& name)
        :IRTSPService(app, name, RTSPServer::RFC7826)
    {}
};


bool RTSPTestServer::Initialize(Promise* promise)
{
    rtspservice_ = new RTSPTestService(this, "test");
    // promise->resolve();
    return true;
}

void RTSPTestServer::On(Promise* promise)
{
    const json& j = promise->meta();
    std::string action = j["action"];
    if (action == "startup") {
        Startup(promise);
    } else if (action == "stop") {
        Stop(promise);
    }
}

void RTSPTestServer::Startup(Promise* promise)
{
    const json& j = promise->data();

    const std::string& launch = j["launch"];
    const std::string& path = j["path"];

    RTSPTestService* rtsp = static_cast<RTSPTestService*>(rtspservice_);
    rtsp->Launch(path, launch, NULL, NULL);
    promise->resolve();
}


void RTSPTestServer::Stop(Promise* promise)
{
    RTSPTestService* rtsp = static_cast<RTSPTestService*>(rtspservice_);
    rtsp->Stop();
    promise->resolve();
}



bool RTSPTestServer::Destroy(Promise* promise)
{
    delete rtspservice_;
    rtspservice_ = NULL;
    return true;
}

