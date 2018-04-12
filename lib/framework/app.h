
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
	class IApp(const std::string& name,WebStreamer* ws)
		: name_(name)
		, pipeline_(NULL)
		, webstreamer_(ws)

	{

	}

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

#define APP( klass )                             \
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

#endif//_LIBWEBSTREAMER_PROCESSOR_H_