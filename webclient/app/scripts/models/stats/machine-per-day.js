/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};
webapp.Models.Stats = webapp.Models.Stats || {};

(function () {
    'use strict';

    webapp.Models.Stats.MachinePerDay = Backbone.ModernModel.extend({
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
                switch (options.trafficMode) {
                    case "both":
                        json.traffic = _.reduce(json.traffic, function (sum, val) { return sum + val; }, 0);
                        json.packets = _.reduce(json.packets, function (sum, val) { return sum + val; }, 0);
                        break;
                    case "in":
                        json.traffic = json.traffic["in"];
                        json.packets = json.packets["in"];
                        break;
                    case "out":
                        json.traffic = json.traffic["out"];
                        json.packets = json.packets["out"];
                        break;
                }
            }

            return json;
        }
    });

})();
