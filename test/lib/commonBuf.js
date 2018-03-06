/**
 * Created by Ganchao on 2018/2/28.
 */

const Common = require('../schema/common_generated').webstreamer;

/**
 * 生成 webstreamer.Channel 的 flatbuf 消息
 * @method channelBuf
 * @param {object} builder flatbuf builder
 * @param {string} name Channel name, audio or video
 * @param {string} codec audio 或 video 编码方式，video为H264,H265,VP8，audio为OPUS,G711A
 * @param {string} mode channel 媒体流模式，sendonly/recvonly/sendrecv
 * @return {flatbuffers.ByteBuffer} channel 消息buf
 */
function channelBuf(builder, name, codec, mode) {
    if(!name || name.toUpperCase() === 'VIDEO' || name.toUpperCase() === 'AUDIO') {
        let nameStr,
            codecStr,
            modeStr;

        if(name) {
            nameStr = builder.createString(name);
        }
        if(codec) {
            codecStr = builder.createString(codec);
        }
        if(mode) {
            modeStr = builder.createString(mode);
        }

        Common.Channel.startChannel(builder);
        if(nameStr) {
            Common.Channel.addName(builder, nameStr);
        }
        if(codecStr) {
            Common.Channel.addCodec(builder, codecStr);
        }
        if(modeStr) {
            Common.Channel.addMode(builder, modeStr);
        }

        return Common.Channel.endChannel(builder);
    } else {
        throw Error('Channel name error, audio or video only!');
    }

}

/**
 * 生成 webstreamer.Endpoint 的 flatbuf 消息
 * @method endpointBuf
 * @param {object} builder flatbuf builder
 * @param {string} name endpoint name, viewer id
 * @param {string} protocol endpoint type, RTSP/WEBRTC
 * @param {string} url endpoint type 为RTSP时对应的url
 * @param {string} initiative endpoint是发送还是接收码流，true为发送，false为接收,默认为true
 * @param {string} videoChannel Video Channel, webstreamer.Channel 的 flatbuf 消息
 * @param {string} audioChannel Audio Channel, webstreamer.Channel 的 flatbuf 消息
 * @return {flatbuffers.ByteBuffer} endpoint 消息buf
 */
function endpointBuf(builder, name, protocol, url, initiative, videoChannel, audioChannel) {
    let nameStr = builder.createString(name.toString()),
        urlStr,
        protocolStr;
    let channels = [];

    if(protocol) {
        protocolStr = builder.createString(protocol);
    }


    if(url) {
        urlStr = builder.createString(url);
    }

    if(videoChannel) {
        channels.push(videoChannel)
    }
    if(audioChannel) {
        channels.push(audioChannel)
    }

    let channelVec = Common.Endpoint.createChannelVector(builder, channels);

    Common.Endpoint.startEndpoint(builder);
    Common.Endpoint.addName(builder, nameStr);
    if(protocolStr) {
        Common.Endpoint.addProtocol(builder, protocolStr);
    }
    if(urlStr) {
        Common.Endpoint.addUrl(builder, urlStr);
    }
    if(initiative) {
        Common.Endpoint.addInitiative(builder, initiative);
    }
    Common.Endpoint.addChannel(builder, channelVec);

    return Common.Endpoint.endEndpoint(builder);
}

/**
 * 生成 webstreamer.Topic 的 flatbuf 消息
 * @method topicBuf
 * @param {object} builder flatbuf builder
 * @param {string} type type of the topic, //WebRTC.JSEP
 * @param {string} component name of the component, stream id
 * @param {string} endpoint name of the endpoint, endpoint id
 * @param {string} content topic conent, format defined according type
 * @return {flatbuffers.ByteBuffer} topic 消息buf
 */
function topicBuf(builder, type, component, endpoint, content) {
    let typeStr = builder.createString(type),
        componentStr = builder.createString(component),
        endpointStr = builder.createString(endpoint),
        contentStr = builder.createString(content);

    Common.Topic.startTopic(builder);
    Common.Topic.addType(builder, typeStr);
    Common.Topic.addComponent(builder, componentStr);
    Common.Topic.addEndpoint(builder, endpointStr);
    Common.Topic.addContent(builder, contentStr);

    return Common.Topic.endTopic(builder);
}

/**
 * 生成 webstreamer.Subscription 的 flatbuf 消息
 * @method subscriptionBuf
 * @param {object} builder flatbuf builder
 * @param {string} typeArray types of the topic, //WebRTC.JSEP
 * @param {string} component name of the component, stream id
 * @param {string} endpoint name of the endpoint, endpoint id
 * @return {flatbuffers.ByteBuffer} topic 消息buf
 */
function subscriptionBuf(builder, component, endpoint, typeArray) {
    let componentStr = builder.createString(component),
        endpointStr = builder.createString(endpoint);

    if(!Array.isArray(typeArray)) {
        throw Error('Parameter error, typeArray should be array');
    }

    let str, strVec = [];
    for(let i = 0; i < typeArray.length; i++) {
        str = builder.createString(typeArray[i]);
        strVec.push(str);
    }
    let typeVec = Common.Subscription.createTypeVector(builder, strVec);

    Common.Subscription.startSubscription(builder);
    Common.Subscription.addType(builder, typeVec);
    Common.Subscription.addComponent(builder, componentStr);
    Common.Subscription.addEndpoint(builder, endpointStr);

    return Common.Subscription.endSubscription(builder);
}

module.exports = {
    channelBuf,
    endpointBuf,
    topicBuf,
    subscriptionBuf
};