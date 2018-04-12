
#ifndef _LIBWEBSTREAMER_ELEMENT_WATCHER_H_
#define _LIBWEBSTREAMER_ELEMENT_WATCHER_H_

#include <nlohmann/json.hpp>
#include <framework/app.h>
class ElementWatcher : public IApp
{
public:
	APP(ElementWatcher)


		ElementWatcher(const std::string& name, WebStreamer* ws)
		: IApp(name, ws)
		, pipeline_(NULL)
	{

	}

	void On(Promise* promise);
	bool Initialize(Promise* promise);
	bool Destroy(Promise* promise);

	virtual void OnMessage(GstBus * bus, GstMessage * message);
protected:
	void Startup(Promise* promise);
	void Stop(Promise* promise);

	void OnSpectrum(const std::string& name, const GstStructure * message);
	void OnMultifilesink(const std::string& name,  const GstStructure * message);

private:

	GstElement * pipeline_;

	nlohmann::json watch_list_;
};


#endif //!_LIBWEBSTREAMER_RTSP_TEST_SERVER_H_