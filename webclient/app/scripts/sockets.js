"use strict";

(function (app) {
    app.SocketConnection = function () {
        return this.construct.apply(this, arguments);
    };

    app.SocketResponse = function () {};

    _.extend(app.SocketConnection.prototype, {
        construct: function (options) {
            var self = this;
            options = options || {};

            self.attributes = _.defaults(options.attributes || {}, _.result(self, "defaultAttributes"));
            self.priv = {
                listeners: {},
                socket: undefined,
                dfds: {
                    connected: new $.Deferred(),
                    error: new $.Deferred()
                }
            };

            var location = self.get("host") + ":" + self.get("port");
            var socket = new WebSocket("ws://" + location);

            socket.onopen = function () {
                self.priv.dfds.connected.resolve();
            };
            socket.onerror = function () {
                self.priv.dfds.connected.reject();
                self.priv.dfds.error.reject();
            };
            socket.onmessage = function (evt) {
                var json;

                try {
                    json = JSON.parse(evt.data);
                } catch (e) {
                    return;
                }

                var uid = json.uid;
                if (self.getListener(uid)) {
                    if (json.error) {
                        self.getListener(uid).reject({ error: json.error });
                    } else {
                        self.getListener(uid).resolve(_.extend((new app.SocketResponse()), json));
                    }
                    self.detachListener(uid);
                }
            };

            self.priv.socket = socket;
        },

        get: function (name) {
            return this.attributes[name];
        },

        set: function (name, value) {
            this.attributes[name] = value;
            return this;
        },

        getListener: function (uid) {
            return this.priv.listeners[uid];
        },
        attachListener: function (uid, dfd) {
            this.priv.listeners[uid] = dfd;
        },
        detachListener: function (uid) {
            delete this.priv.listeners[uid];
        },

        send: function (data, options) {
            var self = this;
            var dfd = new $.Deferred();
            options = options || {};

            self.priv.dfds.error.fail(function () {
                dfd.reject({ error: { connection: true } });
            });

            self.priv.dfds.connected.done(function () {
                if (dfd.state() !== "pending") {
                    return;
                }

                try {
                    if (!_.isObject(data)) {
                        data = JSON.parse(data);
                    }

                    var uid = _.uniqueId("req_");
                    data.uid = uid;

                    data = JSON.stringify(data);

                    self.attachListener(uid, dfd);

                    self.priv.socket.send(data);
                } catch (e) {
                    dfd.reject({ error: { send: { invalidData: true } } });
                }

                var timeoutHandle;
                if (options.timeout) {
                    var timeout = _.result(options, "timeout");

                    timeoutHandle = window.setTimeout(function () {
                        dfd.reject({ error: { timeout: true, delay: timeout } });
                    }, timeout);

                    dfd.done(function () {
                        window.clearTimeout(timeoutHandle);
                    });
                }
            });

            return dfd.promise();
        }
    });
})(window.webapp);
