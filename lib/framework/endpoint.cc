#include "endpoint.h"

IEndpoint::IEndpoint(IApp* app, const std::string& name)
	: app_(app)
	, name_(name)
{

}


IEndpoint::~IEndpoint()
{

}