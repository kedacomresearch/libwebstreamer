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


#ifndef _LIBWEBSTREAMER_FRAMEWORK_ENDPOINT_
#define _LIBWEBSTREAMER_FRAMEWORK_ENDPOINT_

#include "promise.h"

class IApp;
class IEndpoint
{
 public:
    IEndpoint(IApp *app, const std::string &name);
    virtual ~IEndpoint();

    IApp *app() { return app_; }
    const std::string &name() { return name_; }
    virtual bool initialize(Promise *promise) { return true; }
    virtual void terminate() {}
    const std::string &protocol() const { return protocol_; }
    std::string &protocol() { return protocol_; }

 private:
    IApp *app_;
    std::string name_;
    std::string protocol_;
};
#endif
