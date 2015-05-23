/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};
webapp.Models.Stats = webapp.Models.Stats || {};

(function () {
    'use strict';

    webapp.Models.Stats.Machine = Backbone.ModernModel.extend({
        defaults: {},

        urlRoot: "stats-per-machine"
    });

})();
