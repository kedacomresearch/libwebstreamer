#ifndef LIBWEBSTREAMER_MESSAGE_LIVESTREAM_HPP
#define LIBWEBSTREAMER_MESSAGE_LIVESTREAM_HPP

#include "common.hpp"
#include <memory>
#include <vector>

namespace libwebstreamer
{
	namespace message
	{
		namespace livestream
		{
			struct create_t
			{
				common::Pipeline pipeline;
				std::shared_ptr<common::Endpoint> endpoint;
			};

			struct destroy_t
			{
				std::string id;//pipeline id
			};

			struct add_endpoint_t
			{
				std::string id;//pipeline id
				std::shared_ptr<common::Endpoint> endpoint;
			};

			struct remove_endpoint_t
			{
				std::string id;//pipeline id
				std::string endpoint_id;//endpoint id
			};
			
			struct add_endpoints_t
			{
				std::string id;
				std::vector<std::shared_ptr<common::Endpoint>> endpoints;
			};

			struct remove_endpoints_t
			{
				std::string id;//pipeline id
				std::vector<std::string> endpoint_ids;
			};
		}
	}
}

#endif