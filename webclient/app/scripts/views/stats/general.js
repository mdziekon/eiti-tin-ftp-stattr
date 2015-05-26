/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};

(function () {
    'use strict';

    webapp.Views.Stats.GeneralView = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/stats/general.ejs'],

        afterRender: function () {
            var subviews = [
                new webapp.Views.Stats.General.Daily(),
                new webapp.Views.Stats.General.PerMachine()
            ];

            _.each(subviews, function (subview) {
                subview.render();
            });
        }
    });

})();
