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


#ifndef _LIBWEBSTREAMER_PROCESSOR_H_
#define _LIBWEBSTREAMER_PROCESSOR_H_
#include <string>

#include <gst/gst.h>
#include "promise.h"
#include <framework/endpoint.h>
class WebStreamer;
class IApp
{
 public:
    class IApp(const std::string& name, WebStreamer* ws)
        : name_(name)
        , pipeline_(NULL)
        , webstreamer_(ws)

    {}

    virtual bool Initialize(Promise* promise);
    /*
     * return false if the destroy not complete
    */
    virtual bool Destroy(Promise* promise);

    virtual void On(Promise* promise) = 0;

    virtual const char* type() const = 0;
    virtual std::string uname() = 0;

    virtual void Notify(const nlohmann::json& data, const nlohmann::json& meta);
    WebStreamer& webstreamer() { return *webstreamer_; }

 protected:
    std::string name_;
    GstElement* pipeline_;
    WebStreamer* webstreamer_;
};

#define APP(klass)                               \
    static const char* CLASS_NAME() {            \
        return #klass;                           \
    };                                           \
                                                 \
    const char* type() const                     \
    {                                            \
        return CLASS_NAME();                     \
    }                                            \
                                                 \
    std::string uname()                          \
    {                                            \
        return this->name_ + "@" + this->type(); \
    }


#endif  // _LIBWEBSTREAMER_PROCESSOR_H_
