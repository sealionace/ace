"use strict";

var VMouse = (function () {
    function VMouse(file, init) {
        if (typeof file === "undefined") { file = "/sys/devices/platform/vmouse/event"; }
        if (typeof init === "undefined") { init = true; }
        this.file = file;
        this.stream = null;
        this.fs = require("fs");

        if (init)
            this.initStream();
    }
    VMouse.prototype.initStream = function () {
        if (this.stream == null) {
            try  {
                this.stream = this.fs.createWriteStream(this.file, { flags: 'r+' });
            } catch (e) {
                throw new Error("VMouse::init(): file not found [" + this.file + "]");
            }
        }
    };

    VMouse.prototype.checkStream = function (method) {
        if (typeof method === "undefined") { method = "checkStream"; }
        this.initStream();
        /*if (this.stream == null)
        throw new Error("VMouse::" + method + "(): closed stream");*/
    };

    VMouse.prototype.endStream = function () {
        if (this.stream != null) {
            this.stream.end();
            this.stream = null;
        }
    };

    VMouse.prototype.wheel = function (delta, orientation) {
        if (typeof orientation === "undefined") { orientation = "v"; }
        orientation = orientation.toLowerCase();

        if (orientation !== "v" && orientation !== "h") {
            throw new Error("VMouse::wheel(): unsupported orientation '" + orientation + "'");
        }

        this.checkStream();
        this.stream.write("w" + orientation + delta);

        return this;
    };

    // implementei apenas o relativo aqui, pois o absoluto no driver não está funcionando
    VMouse.prototype.move = function (deltaX, deltaY) {
        this.checkStream();
        this.stream.write("mr" + deltaX + " " + deltaY);

        return this;
    };

    VMouse.prototype.click = function (button) {
        if (typeof button === "undefined") { button = "l"; }
        button = button.toLowerCase();

        if (button !== "l" && button != "r" && button != "m") {
            throw new Error("VMouse::click(): invalid button");
        }

        this.checkStream();
        this.stream.write("c" + button);

        return this;
    };

    VMouse.prototype.doubleclick = function (button) {
        if (typeof button === "undefined") { button = "l"; }
        return this.click().click();
    };

    VMouse.prototype.press = function (button) {
        if (typeof button === "undefined") { button = "l"; }
        button = button.toLowerCase();

        if (button !== "l" && button != "r" && button != "m") {
            throw new Error("VMouse::press(): invalid button");
        }

        this.checkStream();
        this.stream.write("cp" + button);

        return this;
    };

    VMouse.prototype.release = function (button) {
        if (typeof button === "undefined") { button = "l"; }
        button = button.toLowerCase();

        if (button !== "l" && button != "r" && button != "m") {
            throw new Error("VMouse::release(): invalid button");
        }

        this.checkStream();
        this.stream.write("cr" + button);

        return this;
    };
    return VMouse;
})();

module.exports = new VMouse();
