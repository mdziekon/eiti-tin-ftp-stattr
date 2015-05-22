/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};

(function () {
    'use strict';

    webapp.Views.Dashboard = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/dashboard.ejs'],

        events: {},

        serialize: function (renderTemplate) {
            var machines = new webapp.Collections.Machine();

            machines.fetch().done(function () {
                renderTemplate({
                    json: {
                        machines: machines.toJSON()
                    }
                });
            }).fail(function () {
                console.error("Failure");
            });
        }
    });

})();
