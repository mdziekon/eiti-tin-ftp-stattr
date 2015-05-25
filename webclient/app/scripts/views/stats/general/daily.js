/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};
webapp.Views.Stats.General = webapp.Views.Stats.General || {};

(function () {
    'use strict';

    webapp.Views.Stats.General.Daily = Backbone.ModernView.extend({

        el: "#page-wrapper .daily-view",

        template: JST['app/scripts/templates/stats/general/daily.ejs'],

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

            view.statsPerDay = new webapp.Collections.Stats.PerDay();

            view.fetchStats({
                data: {
                    lastDays: 7
                }
            }).done(function () {
                renderTemplate({
                    json: {
                        lastDays: view.lastDays,
                        trafficMode: view.trafficMode
                    }
                });
            }).fail(function () {
                renderTemplate({
                    error: {
                        message: "Failed to load daily stats"
                    }
                });
            });
        },

        fetchStats: function (options) {
            var view = this;

            var ajaxReq = [
                view.statsPerDay.fetch(options)
            ];

            return $.when.apply($, ajaxReq);
        },

        afterRender: function () {
            this.createCharts();
        },

        createCharts: function () {
            this.createDailyTrafficChart();
        },

        createDailyTrafficChart: function () {
            var view = this;
            var data = [];
            var machines = {};
            var morrisOptions = {
                element: "stats-chart-daily-traffic",
                xkey: "day",
                pointSize: 3,
                hideHover: 'auto',
                resize: true,
                parseTime: false,
                postUnits: "MB"
            };

            view.statsPerDay.each(function (model) {
                var modelData = model.forTemplate({ trafficMode: view.trafficMode });
                var machinesData = {};
                _.each(modelData.machines, function (machine) {
                    machinesData["machine_" + machine.id] = machine.traffic;

                    if (!machines["machine_" + machine.id]) {
                        machines["machine_" + machine.id] = {
                            id: machine.id,
                            name: machine.name
                        };
                    }
                });

                data.push(_.extend({
                    day: moment(modelData.day).format("MMMM Do")
                }, machinesData));
            });

            var machinesKeys = [];
            var machinesNames = [];
            _.each(machines, function (machine, key) {
                machinesKeys.push(key);
                machinesNames.push(machine.name + " (#" + machine.id + ")");
            });

            Morris.Area(_.extend(
                morrisOptions,
                { data: data },
                {
                    ykeys: machinesKeys,
                    labels: machinesNames
                }
            ));
        },

        changePeriod: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var period = $el.data("period");

            view.lastDays = parseInt(period.replace("last", ""), 10);

            view.fetchStats({
                data: {
                    lastDays: parseInt(period.replace("last", ""), 10)
                },
                submitElement: view.$(".btn-change-period .btn")
            }).done(function () {
                view.$(".btn-change-period .dropdown-toggle .text").text($el.text());
                view.$("#stats-chart-daily-traffic").empty();

                view.createCharts();
            });
        },

        changeTrafficMode: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var trafficMode = $el.data("traffic-mode");

            view.$("#stats-chart-daily-traffic").empty();

            view.trafficMode = trafficMode;
            view.createCharts();
        }
    });

})();
