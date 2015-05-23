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

        syncMachine: function () {
            return this.sync("create", this, {
                patch: true,
                url: this.url() + "/sync",
                noData: true
            });
        },

        toggleSniffer: function () {
            return this.sync("create", this, {
                patch: true,
                url: this.url() + "/toggle-sniffer",
                noData: true
            });
        }
    });

})();
