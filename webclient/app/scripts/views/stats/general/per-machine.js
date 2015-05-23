/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};
webapp.Views.Stats.General = webapp.Views.Stats.General || {};

(function () {
    'use strict';

    webapp.Views.Stats.General.PerMachine = Backbone.ModernView.extend({

        el: "#page-wrapper .per-machine-view",

        template: JST['app/scripts/templates/stats/general/per-machine.ejs'],

        events: {
            "click .btn-change-period li a": "changePeriod"
        },

        defaultLastDays: 7,

        init: function () {
            this.lastDays = this.defaultLastDays;
        },

        serialize: function (renderTemplate) {
            var view = this;

            view.stats = new webapp.Collections.Stats.Machine();

            view.fetchStats({
                data: {
                    lastDays: view.lastDays
                }
            }).done(function () {
                renderTemplate({
                    json: {
                        lastDays: view.lastDays,
                        machines: view.stats.toJSON()
                    }
                });
            }).fail(function () {
                console.error("Failure");
            });
        },

        fetchStats: function (options) {
            var view = this;

            var ajaxReq = [
                view.stats.fetch(options)
            ];

            return $.when.apply($, ajaxReq);
        },

        changePeriod: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var period = $el.data("period");

            view.lastDays = parseInt(period.replace("last", ""), 10);
            view.render();
        }
    });

})();
