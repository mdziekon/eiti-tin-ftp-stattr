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
                renderTemplate({
                    error: {
                        message: "Failed to load machines list"
                    }
                });
            });
        },

        syncMachine: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);
            var flashDfd = new $.Deferred();

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);
            var syncPromise = machine.syncMachine({
                submitElement: $el,
                submitElementFlashDfd: flashDfd
            });

            var promises = [
                syncPromise,
                flashDfd.promise()
            ];

            $.when.apply($, promises).done(function () {
                view.render();
            });
        },

        deleteMachine: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);
            var flashDfd = new $.Deferred();

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);
            var deletePromise = machine.destroy({
                submitElement: $el,
                submitElementFlashDfd: flashDfd
            });

            var promises = [
                deletePromise,
                flashDfd.promise()
            ];

            $.when.apply($, promises).done(function () {
                view.render();
            });
        },

        toggleSniffer: function (evt) {
            var view = this;
            var $el = $(evt.currentTarget);
            var flashDfd = new $.Deferred();

            var machineID = $el.data("machine-id");

            var machine = view.machines.get(machineID);
            var togglePromise = machine.toggleSniffer({
                submitElement: $el,
                submitElementFlashDfd: flashDfd
            });

            var promises = [
                togglePromise,
                flashDfd.promise()
            ];

            $.when.apply($, promises).done(function () {
                view.render();
            });
        }
    });

})();
