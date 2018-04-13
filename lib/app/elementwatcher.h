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

#ifndef _LIBWEBSTREAMER_ELEMENT_WATCHER_H_
#define _LIBWEBSTREAMER_ELEMENT_WATCHER_H_

#include <string>

#include <framework/app.h>

#include <nlohmann/json.hpp>


class ElementWatcher : public IApp
{
 public:
    APP(ElementWatcher);
    ElementWatcher(const std::string& name, WebStreamer* ws)
        : IApp(name, ws)
        , pipeline_(NULL)
    {
    }

    void On(Promise* promise);
    bool Initialize(Promise* promise);
    bool Destroy(Promise* promise);

    virtual void OnMessage(GstBus * bus, GstMessage * message);
 protected:
    void Startup(Promise* promise);
    void Stop(Promise* promise);

    void OnSpectrum(const std::string& name,
        const GstStructure * message);

    void OnMultifilesink(const std::string& name,
        const GstStructure * message);

 private:
    GstElement * pipeline_;
    nlohmann::json watch_list_;
};


#endif  // _LIBWEBSTREAMER_RTSP_TEST_SERVER_H_
