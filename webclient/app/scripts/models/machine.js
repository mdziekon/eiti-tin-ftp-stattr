/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};

(function () {
    'use strict';

    webapp.Models.Machine = Backbone.ModernModel.extend({
        defaults: {},

        postParse: function (response) {
            var data = response;

            if (data.lastSync) {
                data.lastSync *= 1000;
            }

            return data;
        },

        syncMachine: function () {
            return this.sync("update", this, {
                patch: true,
                url: "machine/" + this.id + "/sync",
                noData: true
            });
        }
    });

})();
