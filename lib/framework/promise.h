

#ifndef _LIBWEBSTREAMER_PROMISE_H_
#define _LIBWEBSTREAMER_PROMISE_H_

#include <gst/gst.h>
#include "plugin_interface.h"
#include "nlohmann/json.hpp"

class WebStreamer;
class IApp;
class Promise
{
	struct buffer_t
	{

	};
public:
	Promise(void* iface, const void* context, plugin_callback_fn callback,
		const nlohmann::json& jmeta= nlohmann::json(), 
		const nlohmann::json& jdata= nlohmann::json())
		: iface_((plugin_interface_t *)iface)
		, context_(context)
		, jmeta_(jmeta)
		, jdata_(jdata)
		, responsed_(false)
		, app_(nullptr)
		, webstreamer_(nullptr)
		, callback_(callback)
		, user_data(NULL)
	{
	}

	void resolve(const nlohmann::json& param) {
		if (responsed_) {
			return;//response repated
		}

		const std::string& s = param.dump();
		plugin_buffer_t data;
		plugin_buffer_string_set(&data, s.c_str());
		callback_(iface_, context_, 1, &data);
	}

	void resolve() {
		if (responsed_) {
			return;//response repated
		}

		callback_(iface_, context_, 0,NULL);
	}

	void reject(const std::string& message) {
		if (responsed_) {
			return;//response repated
		}
		plugin_buffer_t data;
		plugin_buffer_string_set(&data, message.c_str());
		callback_(iface_, context_, 1, &data);

	}

	const nlohmann::json& data() const
	{
		return this->jdata_;
	}

	const nlohmann::json& meta() const
	{
		return this->jmeta_;
	}

	//IProcessor* processor() { return processor_; }
	IApp*        app() { return app_;  }
	WebStreamer* webstreamer() {return webstreamer_;}
	void* user_data;
protected:
	void SetWebStreamer(WebStreamer* ws) {
		webstreamer_ = ws;
	}
	void SetApp(IApp* app) {
		app_ = app;
	}
	friend class WebStreamer;

private:

	plugin_interface_t*       iface_;
	const void*               context_;
	nlohmann::json            jdata_;
	nlohmann::json            jmeta_;
	bool                      responsed_;
	WebStreamer*              webstreamer_;
	IApp*                     app_;
	plugin_callback_fn        callback_;


};

#endif//!_LIBWEBSTREAMER_PROMISE_H_