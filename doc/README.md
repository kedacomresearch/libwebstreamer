# Overview

libwebstreamer is plugin (dynamic library) to manipulate media stream.

this plugin is compoosed of compents which provice functions live, push to talk ... 

```
             +------------------------+
             |                        |
stream => [endpoint]               [endpoint]  <==> VLC     
             |         COMPONENT      |
          [endpoint]               [endpoint]  <==> WebBrowser
             |                        |
             +------------------------+
```

* component

  top level element, which includes one or more endpoint. internal this is corresponding pipeline of GStreamer

* endpoint

  endpoint is interface for user to fetch or provide stream for outside module.

## Livestreamer


