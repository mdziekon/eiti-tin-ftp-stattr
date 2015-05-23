/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};

(function () {
    'use strict';

    webapp.Views.MachinesEdit = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/machines/edit.ejs'],

        events: {
            "submit form": "saveMachine"
        },

        serialize: function (renderTemplate) {
            var view = this;

            view.machine = new webapp.Models.Machine({
                id: view.options.machineID
            });

            view.machine.fetch().done(function () {
                renderTemplate({
                    json: {
                        machineID: view.options.machineID,
                        machine: view.machine.toJSON()
                    }
                });
            }).fail(function () {
                console.log("Machine fetch fail");
            });
        },

        saveMachine: function (evt) {
            evt.preventDefault();
            var view = this;

            var $form = view.$("form");

            var machine = view.machine;

            machine.save(
                $form.serializeObject(),
                {
                    wait: true,
                    patch: true,
                    success: function () {
                        console.log(machine);
                        webapp.router.navigate("machines", { trigger: true });
                    },
                    error: function () {
                        console.error("Update error");
                    }
                }
            );
        }
    });

})();
