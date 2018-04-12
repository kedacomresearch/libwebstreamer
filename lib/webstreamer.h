

#ifndef _LIBWEBSTREAMER_WEBSTREAMER_H_
#define _LIBWEBSTREAMER_WEBSTREAMER_H_

#include <stdio.h>
#include <gst/gst.h>

#include "nlohmann/json.hpp"

#include <map>

#include "app/rtsptestserver.h"
#include "app/elementwatcher.h"
#include "app/rtsptestclient.h"
#include "framework/rtspserver.h"
#include "plugin_interface.h"


#define const_error_msg(type,msg) \
    "{\"type\": \"" type "\", \"message\": \"" msg "\"}"



//forward
template<typename... Args>
struct AppFactory;

//baic
template<typename First, typename... Rest>
struct AppFactory<First, Rest...>
{
	//enum { value = Sum<First>::value + Sum<Rest...>::value };
	static IApp* Instantiate(const std::string& type, const std::string& name, WebStreamer* ws)
	{
		if (type == First::CLASS_NAME()) {
			return new First(name,ws);
		}
		else 
		{
			return AppFactory<Rest...>::Instantiate(type, name,ws);
		}
	}

};

//end
template<typename Last>
struct AppFactory<Last>
{
	static IApp* Instantiate(const std::string& type, const std::string& name,WebStreamer* ws)
	{
		if (type == Last::CLASS_NAME() ) {
			return new Last(name,ws);
		}
		return NULL;
	}
};





class RTSPService;

struct plugin_interface_t;
struct plugin_buffer_t;

#define WEBSTREAMER_STATE_IDLE             0
#define WEBSTREAMER_STATE_INITIALIZING   (1 << 2 )
#define WEBSTREAMER_STATE_RUNING         (1 << 3 )
#define WEBSTREAMER_STATE_TERMINATING    (1 << 4 )

class WebStreamer {

public:

	enum class State {
		IDLE,
		INITIALIZING,
		RUNNING,
		TERMINATING,
	};

	WebStreamer(plugin_interface_t* iface);
	//bool Initialize(const nlohmann::json* option, std::string& error);
	void Initialize(Promise* promise);
	//void Terminate();
	void Terminate(Promise* promise);
	//void Terminated();
	void Call(Promise* promise);
	void Notify(plugin_buffer_t* data, plugin_buffer_t* meta);


	static GMainLoop*    main_loop;
	static GMainContext* main_context;
	static gpointer MainloopEntry(Promise* );


	State state() { return state_; }

	std::string InitRTSPServer(const nlohmann::json* option);
	void DestroyRTSPServer();
	RTSPServer* GetRTSPServer(RTSPServer::Type type = RTSPServer::RFC7826)
	{
		if (type >= RTSPServer::SIZE)
		{
			return NULL;
		}
		return rtspserver_[type];
	}

protected:

	typedef AppFactory< RTSPTestServer
		,ElementWatcher
	> Factory;


	void MainLoop(Promise* promise);
	bool Prepare(Promise* promise);
	bool Cleanup();

	void CreateApp(Promise* promise);
	void DestroyApp(Promise* promise);

	void OnPromise(Promise* promise);
	static gboolean OnPromise(gpointer user_data);

	inline IApp* GetApp(const std::string& name, const std::string& type)
	{
		return GetApp(name + "@" + type);
	}

	inline IApp* GetApp(const std::string& uname)
	{
		std::map<std::string, IApp*>::iterator it = apps_.find(uname);
		return   (it == apps_.end()) ? NULL : it->second;

	}
	


private:
	RTSPServer*          rtspserver_[RTSPServer::SIZE];
	GstRTSPSessionPool*  rtsp_session_pool_;
	guint pool_clean_id_;
	std::map<std::string, IApp*> apps_;
	plugin_interface_t* iface_;
	State               state_;
	Promise*            terminate_promise_;
};

#endif