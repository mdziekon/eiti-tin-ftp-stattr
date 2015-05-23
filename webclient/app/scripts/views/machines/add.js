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

            var machine = machines.create(
                $form.serializeObject(),
                {
                    wait: true,
                    success: function () {
                        console.log(machine);
                        webapp.router.navigate("machines", { trigger: true });
                    },
                    error: function () {
                        console.error("Creation error");
                    }
                }
            );
        }
    });

})();
