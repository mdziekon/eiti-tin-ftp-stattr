/*global webapp, Backbone*/

webapp.Routers = webapp.Routers || {};

(function () {
    'use strict';

    webapp.Routers.Main = Backbone.ModernRouter.extend({
        routes: {
            "": "index",
            "/": "index",
            "dashboard": "dashboard",
            "machines": "machines"
        },

        index: function () {
            webapp.router.navigate("dashboard", { trigger: true });
        },

        dashboard: function () {
            var view = new webapp.Views.Dashboard();

            view.render();
        },

        machines: function () {
            var view = new webapp.Views.Machines();

            view.render();
        }
    });

})();
