#ifndef LIBWEBSTREAMER_LIBWEBSTREAMER_HPP
#define LIBWEBSTREAMER_LIBWEBSTREAMER_HPP

#include <glib.h>

#define LIBWEBSTREAMER_NS_BEGIN namespace libwebstreamer {

#define LIBWEBSTREAMER_NS_END }

#define USING_LIBWEBSTREAMER_NS using namespace libwebstreamer;
#define LIBWEBSTREAMER_PREFIX libwebstreamer::
LIBWEBSTREAMER_NS_BEGIN

void initialize(const char* json, size_t size);
void terminate();
void schedule_with_user_data(GSourceFunc callback, gpointer user_data);


LIBWEBSTREAMER_NS_END

#endif