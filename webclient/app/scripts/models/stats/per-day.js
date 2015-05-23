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
        }
    });

})();
