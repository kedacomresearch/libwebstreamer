# Overview
libwebstreamer is plugin (library) of nodejs module webstreamer.
this plugin , which provide many media streamer operation 

## Livestreamer

Livestreamer is set of interfaces that pipes audio/video streams from a source distribute to various viewer via player, such as VLC/Web browser (WebRTC HLS).

```
                                /-------------\
                                |          [viewer] ==> [RTSPClient]
[WebRTC] /[RTSP Server] ==> [source]           |
                                |          [viewer] ==> [WebRTC (browser) ]
                                \-------------/
```
