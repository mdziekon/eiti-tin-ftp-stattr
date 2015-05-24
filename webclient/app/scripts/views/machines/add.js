/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};

(function () {
    'use strict';

    webapp.Views.MachinesAdd = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/machines/add.ejs'],

        events: {
            "submit form": "saveMachine"
        },

        saveMachine: function (evt) {
            evt.preventDefault();
            var view = this;

            var $form = view.$("form");

            var machines = new webapp.Collections.Machine();

            machines.create(
                $form.serializeObject(),
                {
                    wait: true,
                    errorPlaceholder: view.$(".alert-placeholder"),
                    success: function () {
                        webapp.router.navigate("machines", { trigger: true });
                    }
                }
            );
        }
    });

})();
