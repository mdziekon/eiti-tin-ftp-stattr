/*global webapp, Backbone*/

webapp.Models = webapp.Models || {};

(function () {
    'use strict';

    webapp.Models.Machine = Backbone.ModernModel.extend({
        defaults: {},

        command: "get_machine",

        parse: function (response) {
            return _.extend(response, {
                lastSync: response.lastSync * 1000
            });
        }
    });

})();
