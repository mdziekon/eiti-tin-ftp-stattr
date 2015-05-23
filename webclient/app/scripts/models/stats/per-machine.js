/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};
webapp.Models.Stats = webapp.Models.Stats || {};

(function () {
    'use strict';

    webapp.Models.Stats.PerMachine = Backbone.ModernModel.extend({
        defaults: {},

        urlRoot: "stats-per-machine"
    });

})();
