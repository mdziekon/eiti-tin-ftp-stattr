/*global webapp, Backbone*/

webapp.Collections = webapp.Collections || {};

(function () {
    'use strict';

    webapp.Collections.Machine = Backbone.ModernCollection.extend({
        model: webapp.Models.Machine,

        url: "machine",

        postParse: function (response) {
            return response.machines;
        }

    });

})();
