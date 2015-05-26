/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.MachinePerConnection = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.MachinePerConnection,

        url: function () {
            return "stats-machine-per-connection/" + this.options.machineID;
        },

        postParse: function (response) {
            return response.connections;
        },

        forTemplate: function (options) {
            var json = this.map(function (model) {
                return model.forTemplate(options);
            });

            return json;
        }
    });

})();
