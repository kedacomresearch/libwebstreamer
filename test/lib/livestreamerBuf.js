const LiveStreamer = require('../schema/livestreamer_generated').webstreamer.livestreamer;

/**
 * 生成 webstreamer.livestreamer.Create 的flatbuf 消息
 * @method liveStreamerCreateBuf
 * @param {object} builder flatbuf builder
 * @param {string} name name of the livestreamer component,stream id
 * @param {} source 终端信息,webstreamer.Endpoint flatbuf 消息对象
 * @return {flatbuffers.ByteBuffer} liveStreamerCreate 消息buf
 */
function liveStreamerCreateBuf(builder, name, source) {
    let nameStr = builder.createString(name.toString());

    LiveStreamer.Create.startCreate(builder);
    LiveStreamer.Create.addName(builder, nameStr);
    LiveStreamer.Create.addSource(builder, source);

    return LiveStreamer.Create.endCreate(builder);
}

/**
 * 生成 webstreamer.livestreamer.Destroy 的 flatbuf 消息
 * @method liveStreamDestroyBuf
 * @param {object} builder flatbuf builder
 * @param {string} name name of the livestreamer component,stream id
 * @return {flatbuffers.ByteBuffer} LiveStreamDestroy 消息buf
 */
function liveStreamDestroyBuf(builder, name) {
    let nameStr = builder.createString(name);

    LiveStreamer.Destroy.startDestroy(builder);
    LiveStreamer.Destroy.addName(builder, nameStr);

    return LiveStreamer.Destroy.endDestroy(builder);
}

/**
 * 生成 webstreamer.livestreamer.AddViewer 的 flatbuf 消息
 * @method liveStreamAddViewerBuf
 * @param {object} builder flatbuf builder
 * @param {string} component component id
 * @param {object} viewer  webstreamer.Endpoint flatbuf 消息对象
 * @return {flatbuffers.ByteBuffer} liveStreamAddViewer 消息buf
 */
function liveStreamAddViewerBuf(builder, component, viewer) {
    let componentStr = builder.createString(component);

    LiveStreamer.AddViewer.startAddViewer(builder);
    LiveStreamer.AddViewer.addComponent(builder, componentStr);
    LiveStreamer.AddViewer.addViewer(builder, viewer);

    return LiveStreamer.AddViewer.endAddViewer(builder);
}

/**
 * 生成 webstreamer.livestreamer.RemoveViewer 的 flatbuf 消息
 * @method liveStreamRemoveViewerBuf
 * @param {object} builder flatbuf builder
 * @param {string} component component id
 * @param {string} endpoint  endpoint id
 * @return {flatbuffers.ByteBuffer} liveStreamRemoveViewer 消息buf
 */
function liveStreamRemoveViewerBuf(builder, component, endpoint) {
    let componentStr = builder.createString(component),
        endpointStr = builder.createString(endpoint);

    LiveStreamer.RemoveViewer.startRemoveViewer(builder);
    LiveStreamer.RemoveViewer.addComponent(builder, componentStr);
    LiveStreamer.RemoveViewer.addEndpoint(builder, endpointStr);

    return LiveStreamer.RemoveViewer.endRemoveViewer(builder);

}

/**
 * 生成 webstreamer.livestreamer.LiveStreamError 的 flatbuf 消息
 * @method LiveStreamErrorBuf
 * @param {object} builder flatbuf builder
 * @param {int} code error code
 * @param {string} reason  error string
 * @return {flatbuffers.ByteBuffer} LiveStreamError 消息buf
 */
function liveStreamErrorBuf(builder, code, reason) {
    let reasonStr = builder.createString(reason);

    LiveStreamer.LiveStreamError.startLiveStreamError(builder);
    LiveStreamer.LiveStreamError.addCode(builder, code);
    LiveStreamer.LiveStreamError.addReason(builder, reasonStr);

    return LiveStreamer.LiveStreamError.endLiveStreamError(builder);

}

module.exports = {
    liveStreamerCreateBuf,
    liveStreamDestroyBuf,
    liveStreamAddViewerBuf,
    liveStreamRemoveViewerBuf,
    liveStreamErrorBuf
};