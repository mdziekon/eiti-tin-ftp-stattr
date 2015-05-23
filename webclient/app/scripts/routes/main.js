/*global webapp, Backbone*/

webapp.Routers = webapp.Routers || {};

(function () {
    'use strict';

    webapp.Routers.Main = Backbone.ModernRouter.extend({
        routes: {
            "": "index",
            "/": "index",
            "dashboard": "dashboard",

            "machines": "machines",
            "machines/add": "machinesAdd",
            "machines/:machineID/edit": "machinesEdit",

            "stats": "statsGeneral",
            "stats/machine/:machineID": "statsMachine"
        },

        index: function () {
            webapp.router.navigate("dashboard", { trigger: true });
        },

        dashboard: function () {
            var view = new webapp.Views.Dashboard();

            view.render();
        },

        machines: function () {
            var view = new webapp.Views.MachinesList();

            view.render();
        },

        machinesAdd: function () {
            var view = new webapp.Views.MachinesAdd();

            view.render();
        },

        machinesEdit: function (machineID) {
            var view = new webapp.Views.MachinesEdit({
                machineID: machineID
            });

            view.render();
        },

        statsGeneral: function () {
            var view = new webapp.Views.Stats.GeneralView();

            view.render();
        },

        statsMachine: function (machineID) {
            var view = new webapp.Views.Stats.Machine({
                machineID: machineID
            });

            view.render();
        }
    });

})();
