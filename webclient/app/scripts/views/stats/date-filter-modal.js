/*global webapp, Backbone, JST*/

webapp.Views = webapp.Views || {};
webapp.Views.Stats = webapp.Views.Stats || {};

(function () {
    'use strict';

    webapp.Views.Stats.DateFilterView = Backbone.ModernView.extend({
        template: JST['app/scripts/templates/stats/date-filter-modal.ejs'],

        afterRender: function () {
            $('.calendar').datetimepicker(_.extend({
                icons: {
                    time: 'fa fa-clock-o ',
                    date: 'fa fa-calendar',
                    up: 'fa fa-chevron-up',
                    down: 'fa fa-chevron-down',
                    previous: 'fa fa-chevron-left',
                    next: 'fa fa-chevron-right',
                    today: 'fa fa-crosshairs',
                    clear: 'fa fa-trash-o'
                },
                format: "YYYY-MM-DD HH:mm:ss"
            }, this.options.modal.options.datetimepicker));
        },

        getFilters: function () {
            var formData = this.$("form").serializeObject();
            var result = {};

            if (formData.from) {
                result.from = moment(formData.from).unix();
            }
            if (formData.to) {
                result.to = moment(formData.to).unix();
            }

            return result;
        }
    });

    webapp.Views.Stats.DateFilterModal = Backbone.ModalView.extend({
        modalSettings: {
            buttons: {
                "close": {
                    text: "Close",
                    isClose: true,
                    classes: "btn-default"
                },
                "submit": {
                    text: "Filter",
                    isClose: true,
                    classes: "btn-primary",
                    onClick: function (view) {
                        var result = view.innerView.getFilters();

                        if (!_.isEmpty(result)) {
                            view.options.submitDfd.resolve(result);
                        }
                    }
                }
            },
        },

        innerView: new webapp.Views.Stats.DateFilterView()
    });

})();
