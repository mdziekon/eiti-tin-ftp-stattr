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

        defaultPeriod: "last7",
        defaultLastDays: 7,
        defaultTrafficMode: "both",

        init: function () {
            this.period = this.defaultPeriod;
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
                        period: view.period,
                        lastDays: view.lastDays,
                        filterText: "",
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

            if (view.statsPerDay.length == 0)
            {
                view.$("#stats-chart-daily-traffic").html("<div class='text-center'>No data collected</div>");
                return;
            }

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
            var dfd = new $.Deferred();

            if (period === "filter") {
                var modal = new webapp.Views.Stats.DateFilterModal({
                    submitDfd: dfd,
                    datetimepicker: {
                        format: "YYYY-MM-DD"
                    }
                });
                modal.showModal();
                modal.$el.on("hidden.bs.modal", function () {
                    dfd.reject();
                });
            } else {
                dfd.resolve();
            }

            dfd.done(function (filter) {
                if (period !== "filter") {
                    filter = {
                        lastDays: parseInt(period.replace("last", ""), 10)
                    };
                }

                view.fetchStats({
                    data: filter,
                    submitElement: view.$(".btn-change-period .btn"),
                    flashOptions: {
                        complete: {
                            beforeFlash: function () {
                                view.$(".btn-change-period .dropdown-toggle .text").text($el.text());
                            }
                        }
                    }
                }).done(function () {
                    view.$("#stats-chart-daily-traffic").empty();

                    view.updateFilterText(filter);
                    view.createCharts();
                });
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
        },

        updateFilterText: function (filter) {
            var $el = this.$(".period-text");

            if (filter.lastDays) {
                $el.text("Last " + filter.lastDays + " days");
            } else {
                if (!filter.from) {
                    $el.text("Up to " + moment(filter.to * 1000).format("YYYY-MM-DD"));
                } else if (!filter.to) {
                    $el.text("Since " + moment(filter.from * 1000).format("YYYY-MM-DD"));
                } else {
                    $el.text("From " + moment(filter.from * 1000).format("YYYY-MM-DD") + " to " + moment(filter.to * 1000).format("YYYY-MM-DD"));
                }
            }
        }
    });

})();
