/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};
webapp.Views.Stats.Machine = webapp.Views.Stats.Machine || {};

(function () {
    'use strict';

    webapp.Views.Stats.Machine.Daily = Backbone.ModernView.extend({

        el: "#page-wrapper .daily-view",

        template: JST['app/scripts/templates/stats/machine/daily.ejs'],

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

            view.stats = new webapp.Collections.Stats.MachinePerDay(null, {
                machineID: view.options.machineID
            });

            view.fetchStats({
                data: {
                    lastDays: view.lastDays
                }
            }).done(function () {
                renderTemplate({
                    json: {
                        lastDays: view.lastDays,
                        trafficMode: view.trafficMode,
                        stats: view.stats.forTemplate({ trafficMode: view.trafficMode })
                    }
                });
            }).fail(function () {
                renderTemplate({
                    error: {
                        message: "Failed to load machine's stats"
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
            this.$(".table-stats-machine-daily").DataTable({
                responsive: false,
                paging: false,
                searching: false,
                info: false,
                "aoColumns": [
                    { "orderable": false },
                    { "orderable": true },
                    { "orderable": true, "sType": "numeric" }
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
