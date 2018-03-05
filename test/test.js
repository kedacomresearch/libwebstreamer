var Plugin = require('node-plugin').Plugin;

const flatbuffers = require('./schema/flatbuffers.js').flatbuffers;
const commonBuffer = require('./lib/commonBuf.js');
const livestreamerBuffer = require('./lib/livestreamerBuf.js');
const webstreamerBuffer = require('./lib/webstreamerBuf.js');
const Webstreamer = require('./schema/webstreamer_generated.js').webstreamer;
const webstreamerLivestreamer = require('./schema/livestreamer_generated.js').webstreamer.livestreamer;
const webstreamerCommon = require('./schema/common_generated.js').webstreamer;

function notify(buf) {
    console.log(buf.toString());
}

let plugin = new Plugin('libwebstreamer');
let options = {
    plugin: {
        directory: __dirname + '/bin'
    },
    config: 'xxxxxx.json'
}
async function Initialize(arg) {
    await plugin.initialize(arg)
        .then(res => {
            console.log(res);
        })
        .catch(err => {
            console.log(err);
            throw new Error(err.toString());
        })
}


const expr = Buffer.from("100+23", 'utf8');
async function create_livestream(ipc_url) {
    let rtspurl = 'rtsp://172.16.66.65/id=1',
        videoCodec = 'h264',
        videoMode = 'sendonly',
        audioMode = 'sendonly',
        audioCodec = 'pcma',
        component = 'livestream1',
        protocol = 'rtspclient',
        endpoint = 'endpoint1';
    let builder = new flatbuffers.Builder(0),
        videoChannelBuf, audioChannelBuf, endpointBuf, livestreamercreateBuf;

    if (videoCodec) {
        videoChannelBuf = commonBuffer.channelBuf(builder, 'video', videoCodec, videoMode);
    }
    if (audioCodec) {
        audioChannelBuf = commonBuffer.channelBuf(builder, 'audio', audioCodec, audioMode);
    }
    endpointBuf = commonBuffer.endpointBuf(builder, endpoint, protocol, rtspurl, true, videoChannelBuf, audioChannelBuf);
    livestreamercreateBuf = livestreamerBuffer.liveStreamerCreateBuf(builder, component, endpointBuf);

    Webstreamer.root.startroot(builder);
    Webstreamer.root.addAnyType(builder, Webstreamer.Any.webstreamer_livestreamer_Create);
    Webstreamer.root.addAny(builder, livestreamercreateBuf);

    let orc = Webstreamer.root.endroot(builder);

    builder.finish(orc);
    let buf = Buffer.from(builder.asUint8Array());

    await plugin.call(buf)
        .then(res => {
            console.log('~~~~~~~successfully call~~~~~~~');
        })
        .catch(err => {
            throw new Error(err.toString());
        })
}

// async function call2(buf) {
// 	await plugin.call(buf)
// 		.then(res => {
// 			console.log(buf.toString(), '=', res.toString());
// 			return plugin.call(buf);
// 		})
// 		.then(res => {
// 			console.log(buf.toString(), '=', res.toString());
// 		})
// 		.catch(err => {
// 			console.log(err.toString());
// 			throw new Error(err.toString());
// 		})
// }

async function hold_on() {
    await function(){

    }

}
async function Terminate() {
    await plugin.terminate()
        .then(res => {
            console.log(res);
        })
        .catch(err => {
            console.log(err.toString());
            throw new Error(err.toString());
        })
}

async function test(buf) {
    try {
        await Initialize(options);
        console.log('plugin version: ' + plugin.version());
        await create_livestream();
        await hold_on();
        console.log('-----------------------');
        // await call1(buf);
        // await call2(buf);
        // await Terminate();
    } catch (e) {
        console.log('-----------------error----------------');
        console.error(e.message);
        await Terminate();
    }
}

// plugin.on('notify', (buf) => {
//     console.log('notify: ' + buf.toString());
// })

test(expr);

// let rtspurl = 'rtsp://172.16.66.65/id=1',
//     videoCodec = 'h264',
//     videoMode = 'sendonly',
//     audioMode = 'sendonly',
//     audioCodec = 'pcma',
//     component = 'livestream1',
//     protocol = 'rtspclient',
//     endpoint = 'endpoint2';
// let buf = webstreamerBuffer.generateLiveStreamCreateMsgBuf(component, endpoint, protocol, rtspurl, videoCodec, audioCodec, videoMode, audioMode);
// let root = Webstreamer.root.getRootAsroot(buf);

// // let audioChannel = commonBuffer.channelBuf(builder, audioChannelName, audioCodec, audioMode),
// //     videoChannel = commonBuffer.channelBuf(builder, videoChannelName, videoCodec, videoMode),
// //     endpoint = commonBuffer.endpointBuf(builder, endpointName, protocol, url, initiative, videoChannel, audioChannel);

// // endbuilder = livestreamerBuffer.liveStreamerCreateBuf(builder, name, endpoint);
// // builder.finish(endbuilder);
// // buf = builder.dataBuffer();
// // // console.log(builder);
// // msgObj = webstreamerLivestreamer.Create.getRootAsCreate(buf);
// console.log('---------------');
// console.log(root);
// console.log('---------------');
