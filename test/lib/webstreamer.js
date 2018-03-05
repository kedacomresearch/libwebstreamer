/**
 * Created by Ganchao on 2018/3/1.
 */

const Plugin = require('node-plugin').Plugin;
let events = require('events');
const EventEmitter = new events.EventEmitter();

class WebStreamer extends EventEmitter {
    constructor( name ) {
        super();
        this.name = name;
        this.plugin_ = new Plugin(name);
    }

    initialize(options) {
        this.plugin_.initialize(options);
        this.plugin_.on('notify', this.on_notify_);
    }

    on_notify_(buf){
        console.log(buf.toString());
    }

    terminate(){
        return this.plugin_.terminate();
    }

    call_(buf){
        return this.plugin_.call(buf);
    }
}

module.exports = WebStreamer;