/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.MachinePerHour = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.MachinePerHour,

        url: function () {
            return "stats-machine-per-hour/" + this.options.machineID;
        },

        postParse: function (response) {
            return response.stats;
        },

        forTemplate: function (options) {
            var json = this.map(function (model) {
                return model.forTemplate(options);
            });

            return json;
        }
    });

})();
