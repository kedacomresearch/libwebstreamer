#ifndef LIBWEBSTREAMER_DISPATCHER_HPP
#define LIBWEBSTREAMER_DISPATCHER_HPP

namespace libwebstreamer
{
    void call(const void *self, const void *context, const void *data, size_t size);
}

#endif