/**
 * Created by Ganchao on 2018/2/28.
 */
const flatbuffers = require('../schema/flatbuffers').flatbuffers;

const commonBuf = require('./commonBuf');
const livestreamerBuf = require('./livestreamerBuf');
const Webstreamer = require('../schema/webstreamer_generated').webstreamer;


/**
 * 生成LiveStreamCreate的flatbuf 消息
 * @method generateLiveStreamCreateMsgBuf
 * @param {string} component //name of the livestreamer component, component id
 * @param {string} endpoint endpoint name
 * @param {string} protocol endpoint protocol, rtsp or webrtc
 * @param {string} rtspurl IPC的rtsp地址
 * @param {string} videoCodec IPC的视频编码格式
 * @param {string} videoMode IPC的视频媒体流模式，默认为 sendonly
 * @param {string} audioCodec IPC的音频编码格式
 * @param {string} audioMode IPC的音频媒体流模式，默认为 sendonly
 * @return {flatbuffers.ByteBuffer} LiveStreamCreate 消息buf
 */
function generateLiveStreamCreateMsgBuf(component, endpoint, protocol, rtspurl, videoCodec, audioCodec, videoMode = 'sendonly', audioMode = 'sendonly') {
    let builder = new flatbuffers.Builder(0),
        videoChannelBuf, audioChannelBuf, endpointBuf, livestreamercreateBuf;

    if(videoCodec) {
        videoChannelBuf = commonBuf.channelBuf(builder, 'video', videoCodec, videoMode);
    }
    if(audioCodec) {
        audioChannelBuf = commonBuf.channelBuf(builder, 'audio', audioCodec, audioMode);
    }
    endpointBuf = commonBuf.endpointBuf(builder, endpoint, protocol, rtspurl, true, videoChannelBuf, audioChannelBuf);
    livestreamercreateBuf = livestreamerBuf.liveStreamerCreateBuf(builder, component, endpointBuf);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_Create);
    Webstreamer.root.addAny(builder, livestreamercreateBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);

    return builder.dataBuffer();
}

/**
 * 生成 LiveStreamDestroy 的 flatbuf 消息
 * @method generateLiveStreamDestroyMsgBuf
 * @param {string} name livestream name
 * @return {flatbuffers.ByteBuffer} LiveStreamDestroy 消息buf
 */
function generateLiveStreamDestroyMsgBuf(name) {
    let builder = new flatbuffers.Builder(0),
        livestreamerdestroyBuf;

    livestreamerdestroyBuf = livestreamerBuf.liveStreamDestroyBuf(builder, name);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_Destroy);
    Webstreamer.root.addAny(builder, livestreamerdestroyBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);

    return builder.dataBuffer();
}

/**
 * 生成 LiveStreamAddViewer 的 flatbuf 消息
 * @method generateLiveStreamAddViewerMsgBuf
 * @param {string} component livestream name
 * @param {string} endpoint viewer name
 * @param {string} videoCodec  webstreamer.Endpoint  channel codec flatbuf 消息对象
 * @param {string} videoMode  webstreamer.Endpoint  channel mode flatbuf 消息对象
 * @param {string} audioCodec  webstreamer.Endpoint  channel codec flatbuf 消息对象
 * @param {string} audioMode  webstreamer.Endpoint  channel mode flatbuf 消息对象
 * @return {flatbuffers.ByteBuffer} LiveStreamAddViewer 消息buf
 */
function generateLiveStreamAddViewerMsgBuf(component, endpoint, videoCodec, videoMode, audioCodec, audioMode) {
    let builder = new flatbuffers.Builder(0),
        livestreameraddviewerBuf;

    let audioChannel = commonBuf.channelBuf(builder, 'audio', audioCodec, audioMode),
        videoChannel = commonBuf.channelBuf(builder, 'video', videoCodec, videoMode),
        endpointBuf = commonBuf.endpointBuf(builder, endpoint, null, null, null, videoChannel, audioChannel);

    livestreameraddviewerBuf = livestreamerBuf.liveStreamAddViewerBuf(builder, component, endpointBuf);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_AddViewer);
    Webstreamer.root.addAny(builder, livestreameraddviewerBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);

    return builder.dataBuffer();
}

/**
 * 生成 LiveStreamRemoveEndpoint 的 flatbuf 消息
 * @method generateLiveStreamRemoveEndpointMsgBuf
 * @param {string} component livestream name
 * @param {string} endpoint viewer name
 * @return {flatbuffers.ByteBuffer} LiveStreamRemoveViewer 消息buf
 */
function generateLiveStreamRemoveViewerMsgBuf(component, endpoint) {
    let builder = new flatbuffers.Builder(0),
        livestreamerremoveviewerBuf;


    livestreamerremoveviewerBuf = livestreamerBuf.liveStreamRemoveViewerBuf(builder, component, endpoint);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_RemoveViewer);
    Webstreamer.root.addAny(builder, livestreamerremoveviewerBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);

    return builder.dataBuffer();
}

module.exports = {
    generateLiveStreamCreateMsgBuf,
    generateLiveStreamDestroyMsgBuf,
    generateLiveStreamAddViewerMsgBuf,
    generateLiveStreamRemoveViewerMsgBuf
};