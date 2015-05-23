/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};
webapp.Collections.Stats = webapp.Collections.Stats || {};

(function () {
    'use strict';

    webapp.Collections.Stats.Machine = Backbone.ModernCollection.extend({
        model: webapp.Models.Stats.Machine,

        url: "stats-per-machine",

        postParse: function (response) {
            return response.machines;
        }

    });

})();
