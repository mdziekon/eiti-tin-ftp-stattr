/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};

(function () {
    'use strict';

    webapp.Models.Machine = Backbone.ModernModel.extend({
        defaults: {},

        urlRoot: "machine",

        postParse: function (response) {
            var data = response;

            if (data.lastSync) {
                data.lastSync *= 1000;
            }

            return data;
        },

        syncMachine: function (options) {
            return this.sync("create", this, _.defaults({
                patch: true,
                url: this.url() + "/sync",
                noData: true
            }, options));
        },

        pingMachine: function (options) {
            return this.sync("create", this, _.defaults({
                patch: true,
                url: this.url() + "/ping",
                noData: true
            }, options));
        },

        toggleSniffer: function (options) {
            return this.sync("create", this, _.defaults({
                patch: true,
                url: this.url() + "/toggle-sniffer",
                noData: true
            }, options));
        }
    });

})();
