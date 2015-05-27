/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};
webapp.Views.Stats.Machine = webapp.Views.Stats.Machine || {};

(function () {
    'use strict';

    webapp.Views.Stats.Machine.PerHour = Backbone.ModernView.extend({

        el: "#page-wrapper .per-hour-view",

        template: JST['app/scripts/templates/stats/machine/per-hour.ejs'],

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

            view.stats = new webapp.Collections.Stats.MachinePerHour(null, {
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
            this.createCharts();
        },

        createCharts: function () {
            this.createDailyTrafficChart();
        },

        createDailyTrafficChart: function () {
            var view = this;
            var data = {
                traffic: [],
                packets: []
            };
            var position = "right";

            if (view.stats.length == 0)
            {
                view.$("#stats-chart-hourly-traffic").html("<div class='text-center'>No data collected</div>");
                return;
            }

            view.stats.each(function (model) {
                var modelData = model.forTemplate({ trafficMode: view.trafficMode });

                data.traffic.push([ modelData.hour, modelData.traffic ]);
                data.packets.push([ modelData.hour, modelData.packets ]);
            });

            $.plot(view.$("#stats-chart-hourly-traffic"), [{
                data: data.traffic,
                label: "Traffic",
                lines: {
                    show: true,
                    fill: true
                },
                points: {
                    show: true
                }
            }, {
                data: data.packets,
                label: "Packets",
                yaxis: 2,
                lines: {
                    show: true
                },
                points: {
                    show: true
                },
            }], {
                xaxes: [{
                    ticks: _.range(0, 24, 1),
                    tickFormatter: function (v, axis) {
                        var start = (v < 10) ? "0" + v : v;
                        return "" + start;
                    }
                }],
                yaxes: [{
                    tickFormatter: function (v, axis) {
                        return v.toFixed(axis.tickDecimals) + " B";
                    }
                }, {
                    // align if we are to the right
                    alignTicksWithAxis: position == "right" ? 1 : null,
                    position: position
                }],
                legend: {
                    position: 'sw'
                },
                grid: {
                    hoverable: true //IMPORTANT! this is needed for tooltip to work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s: %y"
                },
                series: {
                    curvedLines: {
                        active: true
                    }
                },
            });
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

                view.createCharts();
            });
        },

        changeTrafficMode: function (evt) {
            evt.preventDefault();

            var view = this;
            var $el = $(evt.currentTarget);
            var trafficMode = $el.data("traffic-mode");

            view.$(".btn-change-traffic-mode .dropdown-toggle .text").text($el.text());

            view.trafficMode = trafficMode;
            view.createCharts();
        }
    });

})();
