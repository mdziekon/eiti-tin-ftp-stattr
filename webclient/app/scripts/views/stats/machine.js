/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};

(function () {
    'use strict';

    webapp.Views.Stats.MachineView = Backbone.ModernView.extend({

        el: "#page-wrapper",

        template: JST['app/scripts/templates/stats/machine.ejs'],

        serialize: function (renderTemplate) {
            var view = this;

            view.machine = new webapp.Models.Machine({
                id: view.options.machineID
            });

            view.machine.fetch().done(function () {
                renderTemplate({
                    json: {
                        machine: view.machine.toJSON()
                    }
                });
            }).fail(function () {
                console.log("Machine fetch fail");
            });
        },

        afterRender: function () {
            var subviews = [
                new webapp.Views.Stats.Machine.Daily({
                    machineID: this.options.machineID
                })
            ];

            _.each(subviews, function (subview) {
                subview.render();
            });
        }
    });

})();
