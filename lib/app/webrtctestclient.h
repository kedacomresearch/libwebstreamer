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

#ifndef _LIBWEBSTREAMER_WEBRTC_TEST_CLIENT_H_
#define _LIBWEBSTREAMER_WEBRTC_TEST_CLIENT_H_

#include <endpoint/webrtc.h>
#include <app/elementwatcher.h>

class WebRTCTestClient : public ElementWatcher
{
 public:
    APP(WebRTCTestClient)
    WebRTCTestClient(const std::string &name, WebStreamer *ws)
        : ElementWatcher(name, ws)
        , webrtc_ep_(NULL)
    {
    }

    virtual void On(Promise *promise);
    virtual bool Initialize(Promise *promise);
    virtual bool Destroy(Promise *promise);

 private:
    void Startup(Promise *promise);
    void Stop(Promise *promise);
    void set_remote_description(Promise *promise);
    void set_remote_candidate(Promise *promise);

    WebRTC *webrtc_ep_;
};
#endif
