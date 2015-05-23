/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};

(function () {
    'use strict';

    webapp.Views.Stats.General = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/stats/general.ejs'],

        events: {},

        serialize: function (renderTemplate) {
            var view = this;

            view.statsPerDay = new webapp.Collections.Stats.PerDay();

            var ajaxReq = [
                view.statsPerDay.fetch()
            ];

            $.when.apply($, ajaxReq).done(function () {
                renderTemplate({
                    json: {
                        perDay: view.statsPerDay.toJSON()
                    }
                });
            }).fail(function () {
                console.error("Failure");
            });
        },

        afterRender: function () {
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
                var modelData = model.toJSON();
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
        }
    });

})();
