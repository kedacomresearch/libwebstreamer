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

#include <string>
#include <map>
#include "typedef.h"


std::map<std::string, EndpointType> endpoint_type = {{"rtspclient", EndpointType::RTSP_CLIENT},
                                                     {"rtspserver", EndpointType::RTSP_SERVER},
                                                     {"testsink", EndpointType::TEST_SINK},
                                                     {"webrtc", EndpointType::WEBRTC}};
EndpointType get_endpoint_type(const std::string &type)
{
    return endpoint_type[type];
}
std::map<std::string, VideoEncodingType> video_encoding_type = {{"h264", VideoEncodingType::H264},
                                                                {"vp8", VideoEncodingType::VP8},
                                                                {"h265", VideoEncodingType::H265}};
VideoEncodingType get_video_encoding_type(const std::string &type)
{
    return video_encoding_type[type];
}
std::map<std::string, AudioEncodingType> audio_encoding_type = {{"pcma", AudioEncodingType::PCMA},
                                                                {"pcmu", AudioEncodingType::PCMU},
                                                                {"opus", AudioEncodingType::OPUS}};
AudioEncodingType get_audio_encoding_type(const std::string &type)
{
    return audio_encoding_type[type];
}

std::string uppercase(const std::string target)
{
    std::string result = target;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] >= 97 && result[i] <= 122) {
            result[i] -= 32;
        }
    }
    return result;
}
