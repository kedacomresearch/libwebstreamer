#ifndef LIBWEBSTREAMER_PIPELINE_FRAMEWORK_PIPELINE_HPP
#define LIBWEBSTREAMER_PIPELINE_FRAMEWORK_PIPELINE_HPP

#include <message/common.hpp>
#include "endpoint.hpp"
namespace libwebstreamer
{
	namespace pipeline
	{
		class Pipeline
		{
		public:
			Pipeline(const message::common::Pipeline& params);
			virtual ~Pipeline();

		public:
			const std::string& id() const;

		private:
			std::vector<std::shared_ptr<Endpoint>> endpoints;

		private:
			std::string id_;
		};
	}
}

#endif