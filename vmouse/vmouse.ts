declare var module;
declare var require;
"use strict";

class VMouse {
	
	private fs;
	private stream = null;
	
	constructor (private file = "/sys/devices/platform/vmouse/event", init = true) {
		this.fs = require("fs");
		
		if (init)
			this.initStream();
	}
	
	initStream () {
		if (this.stream == null) {
			try {
				this.stream = this.fs.createWriteStream(this.file, { flags: 'r+' });
			} catch (e) {
				throw new Error("VMouse::init(): file not found [" + this.file + "]");
			}
		}
	}
	
	checkStream (method = "checkStream") {
		this.initStream();
		/*if (this.stream == null)
			throw new Error("VMouse::" + method + "(): closed stream");*/
	}
	
	endStream () {
		if (this.stream != null) {
			this.stream.end();
			this.stream = null;
		}
	}
	
	wheel (delta: number, orientation = "v") {
		orientation = orientation.toLowerCase();
		
		if (orientation !== "v" && orientation !== "h") {
			throw new Error("VMouse::wheel(): unsupported orientation '" + orientation + "'");
		}
		
		this.checkStream();
		this.stream.write("w" + orientation + delta);
		
		return this;
	}
	
	// implementei apenas o relativo aqui, pois o absoluto no driver não está funcionando
	move (deltaX: number, deltaY: number) {
		this.checkStream();
		this.stream.write("mr" + deltaX + " " + deltaY);
		
		return this;
	}
	
	click (button = "l") {
		button = button.toLowerCase();
		
		if (button !== "l" && button != "r" && button != "m") {
			throw new Error("VMouse::click(): invalid button");
		}
		
		this.checkStream();
		this.stream.write("c" + button);
		
		return this;
	}
	
	doubleclick (button = "l") {
		return this.click().click();
	}
	
	press (button = "l") {
		button = button.toLowerCase();
		
		if (button !== "l" && button != "r" && button != "m") {
			throw new Error("VMouse::press(): invalid button");
		}
		
		this.checkStream();
		this.stream.write("cp" + button);
		
		return this;
	}
	
	release (button = "l") {
		button = button.toLowerCase();
		
		if (button !== "l" && button != "r" && button != "m") {
			throw new Error("VMouse::release(): invalid button");
		}
		
		this.checkStream();
		this.stream.write("cr" + button);
		
		return this;
	}
	
}

module.exports = new VMouse();