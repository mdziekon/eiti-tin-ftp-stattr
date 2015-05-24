/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.PerDay = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.PerDay,

        url: "stats-per-day",

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
