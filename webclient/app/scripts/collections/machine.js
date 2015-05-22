/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};

(function () {
    'use strict';

    webapp.Collections.Machine = Backbone.ModernCollection.extend({
        model: webapp.Models.Machine,

        command: "list_machines",

        parse: function (response) {
            return response.data.machines;
        }

    });

})();
