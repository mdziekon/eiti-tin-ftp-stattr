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
            "click .btn-change-period li a": "changePeriod",
            "click .btn-change-traffic-mode li a": "changeTrafficMode"
        },

        defaultLastDays: 7,
        defaultTrafficMode: "both",

        init: function () {
            this.lastDays = this.defaultLastDays;
            this.trafficMode = this.defaultTrafficMode;
        },

        serialize: function (renderTemplate) {
            var view = this;

            view.stats = new webapp.Collections.Stats.PerMachine();

            view.fetchStats({
                data: {
                    lastDays: view.lastDays
                }
            }).done(function () {
                renderTemplate({
                    json: {
                        lastDays: view.lastDays,
                        trafficMode: view.trafficMode,
                        machines: view.stats.forTemplate({
                            trafficMode: view.trafficMode
                        })
                    }
                });
            }).fail(function () {
                renderTemplate({
                    error: {
                        message: "Failed to load machines' stats"
                    }
                });
            });
        },

        fetchStats: function (options) {
            var view = this;

            var ajaxReq = [
                view.stats.fetch(options)
            ];

            return $.when.apply($, ajaxReq);
        },

        afterRender: function () {
            if (this.stats.length == 0)
            {
                return;
            }
            
            this.$(".table-stats-per-machine").DataTable({
                responsive: false,
                paging: false,
                searching: false,
                info: false,
                "aoColumns": [
                    { "orderable": false },
                    { "orderable": false },
                    { "orderable": true }
                ]
            });
        },

        changePeriod: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var period = $el.data("period");

            view.lastDays = parseInt(period.replace("last", ""), 10);
            view.render();
        },

        changeTrafficMode: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var trafficMode = $el.data("traffic-mode");

            view.trafficMode = trafficMode;
            view.render();
        }
    });

})();
