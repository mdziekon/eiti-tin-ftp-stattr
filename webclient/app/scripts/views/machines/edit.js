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
                renderTemplate({
                    error: {
                        message: "Failed to load machine's data"
                    }
                });
            });
        },

        saveMachine: function (evt) {
            evt.preventDefault();
            var view = this;

            var $form = view.$("form");
            var formData = $form.serializeObject();
            formData.port = parseInt(formData.port, 10);

            var machine = view.machine;

            machine.save(
                formData,
                {
                    wait: true,
                    patch: true,
                    errorPlaceholder: view.$(".alert-placeholder"),
                    success: function () {
                        webapp.router.navigate("machines", { trigger: true });
                    }
                }
            );
        }
    });

})();
