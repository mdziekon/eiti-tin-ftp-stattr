/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.MachinePerDay = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.MachinePerDay,

        url: function () {
            return "stats-machine-per-day/" + this.options.machineID;
        },

        postParse: function (response) {
            return response.stats;
        }

    });

})();
