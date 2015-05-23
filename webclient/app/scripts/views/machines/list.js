/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};

(function () {
    'use strict';

    webapp.Views.MachinesList = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/machines/list.ejs'],

        events: {
            "click .btn-sync": "syncMachine",
            "click .btn-delete": "deleteMachine",
            "click .btn-toggle-sniffer": "toggleSniffer"
        },

        serialize: function (renderTemplate) {
            var view = this;
            view.machines = new webapp.Collections.Machine();

            view.machines.fetch().done(function () {
                renderTemplate({
                    json: {
                        machines: view.machines.toJSON()
                    }
                });
            }).fail(function () {
                console.error("Failure");
            });
        },

        syncMachine: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);

            machine.syncMachine().done(function () {
                view.render();
            }).fail(function () {
                console.error("Sync failed");
            });
        },

        deleteMachine: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);

            machine.destroy().done(function () {
                view.render();
            }).fail(function () {
                console.error("Deletion failed");
            });
        },

        toggleSniffer: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);

            machine.toggleSniffer().done(function () {
                view.render();
            }).fail(function () {
                console.error("Toggle failed");
            });
        }
    });

})();
