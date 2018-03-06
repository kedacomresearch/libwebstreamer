const flatbuffers = require('./schema/flatbuffers.js').flatbuffers;
const commonBuffer = require('./lib/commonBuf.js');
const livestreamerBuffer = require('./lib/livestreamerBuf.js');
const webstreamerBuffer = require('./lib/webstreamerBuf.js');
const Webstreamer = require('./schema/webstreamer_generated.js').webstreamer;
const webstreamerLivestreamer = require('./schema/livestreamer_generated.js').webstreamer.livestreamer;
const webstreamerCommon = require('./schema/common_generated.js').webstreamer;

function Create(options) {
    let builder = new flatbuffers.Builder(0),
        videoChannelBuf, audioChannelBuf, endpointBuf, livestreamercreateBuf;

    if (options.videoCodec) {
        videoChannelBuf = commonBuffer.channelBuf(builder, 'video', options.videoCodec, options.videoMode);
    }
    if (options.audioCodec) {
        audioChannelBuf = commonBuffer.channelBuf(builder, 'audio', options.audioCodec, options.audioMode);
    }
    endpointBuf = commonBuffer.endpointBuf(builder, options.endpoint, options.protocol, options.rtspurl, options.initiative, videoChannelBuf, audioChannelBuf);
    livestreamercreateBuf = livestreamerBuffer.liveStreamerCreateBuf(builder, options.component, endpointBuf);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_Create);
    Webstreamer.root.addAny(builder, livestreamercreateBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);
    let buf = Buffer.from(builder.asUint8Array());
    return buf;
}
function Destroy(component) {
    let builder = new flatbuffers.Builder(0),
        livestreamerdestroyBuf;

    livestreamerdestroyBuf = livestreamerBuffer.liveStreamDestroyBuf(builder, component);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_Destroy);
    Webstreamer.root.addAny(builder, livestreamerdestroyBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);
    let buf = Buffer.from(builder.asUint8Array());
    return buf;
}
function AddViewer(options) {
    let builder = new flatbuffers.Builder(0),
        livestreameraddviewerBuf;

    let audioChannel = commonBuffer.channelBuf(builder, 'audio', options.audioCodec, options.audioMode),
        videoChannel = commonBuffer.channelBuf(builder, 'video', options.videoCodec, options.videoMode),
        endpointBuf = commonBuffer.endpointBuf(builder, options.endpoint, options.protocol, options.rtspurl, options.initiative, options.videoChannel, options.audioChannel);

    livestreameraddviewerBuf = livestreamerBuffer.liveStreamAddViewerBuf(builder, options.component, endpointBuf);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_AddViewer);
    Webstreamer.root.addAny(builder, livestreameraddviewerBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);
    let buf = Buffer.from(builder.asUint8Array());
    return buf;
}

function RemoveViewer(component, endpoint) {
    let builder = new flatbuffers.Builder(0),
        livestreamerremoveviewerBuf;


    livestreamerremoveviewerBuf = livestreamerBuffer.liveStreamRemoveViewerBuf(builder, component, endpoint);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_RemoveViewer);
    Webstreamer.root.addAny(builder, livestreamerremoveviewerBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);
    let buf = Buffer.from(builder.asUint8Array());
    return buf;
}

module.exports = {
    Create,
    Destroy,
    AddViewer,
    RemoveViewer
}