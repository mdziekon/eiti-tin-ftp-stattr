/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.PerMachine = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.PerMachine,

        url: "stats-per-machine",

        postParse: function (response) {
            return response.machines;
        },

        forTemplate: function (options) {
            var json = this.map(function (model) {
                return model.forTemplate(options);
            });

            return json;
        }
    });

})();
