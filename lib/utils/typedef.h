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

#ifndef _LIBWEBSTREAMER_UTILS_TYPE_DEFINE_H_
#define _LIBWEBSTREAMER_UTILS_TYPE_DEFINE_H_

#include <string>
enum EndpointType
{
    RTSP_CLIENT = (1 << 0),
    RTSP_SERVER = (1 << 1),
    TEST_SINK = (1 << 2),
    WEBRTC = (1 << 3)
};
enum VideoEncodingType
{
    H264 = (1 << 0),
    VP8 = (1 << 1),
    H265 = (1 << 2)
};

enum AudioEncodingType
{
    PCMA = (1 << 0),
    PCMU = (1 << 1),
    OPUS = (1 << 2)
};

EndpointType get_endpoint_type(const std::string &type);
VideoEncodingType get_video_encoding_type(const std::string &type);
AudioEncodingType get_audio_encoding_type(const std::string &type);

std::string uppercase(const std::string target);

#endif
