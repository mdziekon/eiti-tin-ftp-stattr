/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};
webapp.Models.Stats = webapp.Models.Stats || {};

(function () {
    'use strict';

    webapp.Models.Stats.PerDay = Backbone.ModernModel.extend({
        defaults: {},

        postParse: function (response) {
            return _.extend(response, {
                day: response.day * 1000
            });
        },

        forTemplate: function (options) {
            var json = _.clone(this.toJSON(), true);
            options = options || {};

            if (options.trafficMode) {
                _.each(json.machines, function (machine) {
                    switch (options.trafficMode) {
                        case "both":
                            machine.traffic = _.reduce(machine.traffic, function (sum, val) { return sum + val; }, 0);
                            break;
                        case "in":
                            machine.traffic = machine.traffic["in"];
                            break;
                        case "out":
                            machine.traffic = machine.traffic["out"];
                            break;
                    }
                });
            }

            return json;
        }
    });

})();
