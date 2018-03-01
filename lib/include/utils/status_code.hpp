#ifndef LIBWEBSTREAMER_UTIL_HTTP_STATUS_CODE_HPP
#define LIBWEBSTREAMER_UTIL_HTTP_STATUS_CODE_HPP

#include <string>

namespace libwebstreamer
{
    namespace util
    {
        //see http://www.restapitutorial.com/httpstatuscodes.html
        //or  https://www.ietf.org/assignments/http-status-codes/http-status-codes.xml
        enum class status_code
        {
            Continue = 100,
            Switching_Protocols = 101,
            Processing = 102,

            OK = 200,
            Created = 201,
            Accepted = 202,
            Non_Authoritative_Information = 203,
            No_Content = 204,
            Reset_Content = 205,
            Partial_Content = 206,
            Multi_Status = 207,
            Already_Reported = 208,

            IM_Used = 226,

            Multiple_Choices = 300,
            Moved_Permanently = 301,
            Found = 302,
            See_Other = 303,
            Not_Modified = 304,
            Use_Proxy = 305,

            Temporary_Redirect = 307,
            Permanent_Redirect = 308,

            Bad_Request = 400,
            Unauthorized = 401,
            Payment_Required = 402,
            Forbidden = 403,
            Not_Found = 404,
            Method_Not_Allowed = 405,
            Not_Acceptable = 406,
            Proxy_Authentication_Required = 407,
            Request_Timeout = 408,
            Conflict = 409,
            Gone = 410,
            Length_Required = 411,
            Precondition_Failed = 412,
            Payload_Too_Large = 413,
            URI_Too_Long = 414,
            Unsupported_Media_Type = 415,
            Range_Not_Satisfiable = 416,
            Expectation_Failed = 417,

            Misdirected_Request = 421,
            Unprocessable_Entity = 422,
            Locked = 423,
            Failed_Dependency = 424,

            Upgrade_Required = 426,

            Precondition_Required = 428,
            Too_Many_Requests = 429,

            Request_Header_Fields_Too_Large = 431,

            Unavailable_For_Legal_Reasons = 451,

            Internal_Server_Error = 500,
            Not_Implemented = 501,
            Bad_Gateway = 502,
            Service_Unavailable = 503,
            Gateway_Timeout = 504,
            HTTP_Version_Not_Supported = 505,
            Variant_Also_Negotiates = 506,
            Insufficient_Storage = 507,
            Loop_Detected = 508,

            Not_Extended = 510,
            Network_Authentication_Required = 511

        };

        static inline const char *const reason(int code)
        {
            static const std::map<int, const char *const> reason_map = {
                {100, "Continue"},
                {101, "Switching Protocols"},
                {102, "Processing"},

                {200, "OK"},
                {201, "Created"},
                {202, "Accepted"},
                {203, "Non-Authoritative Information"},
                {204, "No Content"},
                {205, "Reset Content"},
                {206, "Partial Content"},
                {207, "Multi-Status"},
                {208, "Already Reported"},

                {226, "IM Used"},

                {300, "Multiple Choices"},
                {301, "Moved Permanently"},
                {302, "Found"},
                {303, "See Other"},
                {304, "Not Modified"},
                {305, "Use Proxy"},

                {307, "Temporary Redirect"},
                {308, "Permanent Redirect"},

                {400, "Bad Request"},
                {401, "Unauthorized"},
                {402, "Payment Required"},
                {403, "Forbidden"},
                {404, "Not Found"},
                {405, "Method Not Allowed"},
                {406, "Not Acceptable"},
                {407, "Proxy Authentication Required"},
                {408, "Request Timeout"},
                {409, "Conflict"},
                {410, "Gone"},
                {411, "Length Required"},
                {412, "Precondition Failed"},
                {413, "Payload Too Large"},
                {414, "URI Too Long"},
                {415, "Unsupported Media Type"},
                {416, "Range Not Satisfiable"},
                {417, "Expectation Failed"},

                {421, "Misdirected Request"},
                {422, "Unprocessable Entity"},
                {423, "Locked"},
                {424, "Failed Dependency"},

                {426, "Upgrade Required"},

                {428, "Precondition Required"},
                {429, "Too Many Requests"},

                {431, "Request Header Fields Too Large"},

                {451, "Unavailable For Legal Reasons"},

                {500, "Internal Server Error"},
                {501, "Not Implemented"},
                {502, "Bad Gateway"},
                {503, "Service Unavailable"},
                {504, "Gateway Timeout"},
                {505, "HTTP Version Not Supported"},
                {506, "Variant Also Negotiates"},
                {507, "Insufficient Storage"},
                {508, "Loop Detected"},

                {510, "Not Extended"},
                {511, "Network Authentication Required"}};
            std::map<int, const char *const>::const_iterator it = reason_map.find(code);
            return it == reason_map.cend() ? "" : it->second;
        }

        static inline const char *const reason(status_code code)
        {
            return reason(static_cast<int>(code));
        }
    }
}
#endif