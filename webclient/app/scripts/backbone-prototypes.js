"use strict";

(function (Backbone, app) {
    Backbone.ModernRouter = Backbone.Router.extend({
        initialize: function (options) {
            this.views = {};

            this.init.apply(this, options);
        },

        init: function (options) {
            // Override this at will
        },

        route: function (route, name, callback) {
            var router = this;

            if (!callback) {
                callback = router[name];
            }

            return Backbone.Router.prototype.route.call(router, route, name, function () {
                route = route || "";
                var section = route.match(/^([^/]+)/);
                section = (section || [])[1];
                section = section || "";
                section = section.replace(/[\W]/g, "");

                app.changeSection(section);

                callback.apply(router, arguments);
            });
        }
    });

    Backbone.ModernView = Backbone.View.extend({
        errorTemplate: JST['app/scripts/templates/error.ejs'],

        initialize: function (options) {
            this._init.apply(this, arguments);

            this.init.apply(this, arguments);
        },

        _init: function (options) {
            this.options = options || {};

            $(this.$el).off();
        },

        init: function (options) {
            // Override this at will
        },

        show: function () {
            this.$el.removeClass("hidden");
        },

        hide: function () {
            this.$el.addClass("hidden");
        },

        render: function (renderOptions) {
            var view = this;
            renderOptions = _.defaults(renderOptions || {}, {
                test: true
            });

            this.$el.removeClass("hidden");

            var renderTemplate = function (options) {
                options = options || {};

                view.$el.spin(false);

                if (!options.error) {
                    view.$el.html(view.template(options.json));

                    view.afterRender();
                } else {
                    view.$el.html(view.errorTemplate(options.error));
                }
            };

            view.$el.html("");
            view.$el.spin();
            view.serialize(renderTemplate);

            return this;
        },

        serialize: function (renderTemplate) {
            // Override this at will
            renderTemplate();
        },

        afterRender: function () {
            // Override this at will
        }
    });

    Backbone.ModalView = Backbone.ModernView.extend({
        defaultModalSettings: {
            id: "generic-modal",
            title: "Modal",
            closeButton: true,
            buttons: {
                "close": {
                    text: "Close",
                    isClose: true,
                    classes: "btn-default",
                    data: {},
                    onClick: undefined
                }
            },

            backdrop: true,
            keyboard: false
        },

        el: "#modal-placeholder",
        template: "common/templates/modal.html",

        innerView: undefined,

        _init: function (options) {
            var view = this;

            Backbone.ModernView.prototype._init.apply(view, options);

            view.modalSettings = _.defaults(view.modalSettings || {}, view.defaultModalSettings);

            view.innerView.options.modal = view;
        },

        serialize: function (renderTemplate) {
            var view = this;

            renderTemplate({
                json: {
                    modal: view.modalSettings
                }
            });
        },

        afterRender: function () {
            var view = this;
            var $footer = view.$(".modal-footer");

            var newEvents = {};
            var btnIdx = 0;
            _.each(view.modalSettings.buttons, function (button, name) {
                var btnSelector = ".btn:nth-child(" + (btnIdx + 1) + ")";
                var $btn = $footer.find(btnSelector);

                button.$el = $btn;

                if (button.onClick && _.isFunction(button.onClick)) {
                    var eventKey = "click .modal-footer " + btnSelector;
                    newEvents[eventKey] = _.partial(button.onClick, _, view);
                }

                btnIdx++;
            });
            if (!_.isEmpty(newEvents)) {
                view.delegateEvents(_.extend(view.events || {}, newEvents));
            }

            var $modal = view.$(".modal-box");

            $modal.modal(view.modalSettings);
            $modal.on("hidden.bs.modal", function () {
                view.undelegateEvents();
            });

            view.$(".modal-body").spin();

            view.innerView.setElement(view.$(".modal-body"));
            view.innerView.render({ force: true });
        },

        showModal: function (renderOptions) {
            renderOptions = _.defaults(renderOptions || {}, {});

            return this.render(_.extend(renderOptions, {
                force: true
            }));
        },

        hideModal: function () {
            var $modal = this.$(".modal-box");
            $modal.modal("hide");
        }
    });

    Backbone.sync = function (method, object, options) {
        options = options || {};

        var type;
        switch (method) {
            case "create":
                type = "POST";
                break;
            case "read":
                type = "GET";
                break;
            case "update":
                type = "PUT";
                break;
            case "patch":
                type = "PATCH";
                break;
            case "delete":
                type = "DELETE";
                break;
        }


        var data = {
            route: _.result(options, "url") || _.result(object, "url"),
            type: type
        };

        if (!options.data && !options.noData) {
            if (type === "POST" || type === "PUT") {
                _.extend(data, {
                    data: object.toJSON()
                });
            } else if (type === "PATCH" && !(object instanceof Backbone.Collection)) {
                _.extend(data, {
                    data: options.attrs
                });
            }
        } else if (options.data) {
            data.data = options.data;
        }

        var promise = app.connection.send(data);

        promise.done(options.success || function () {});
        promise.fail(options.error || function () {});

        return promise;
    };

    Backbone.ModernModel = Backbone.Model.extend({
        initialize: function (attributes, options) {
            this._init.apply(this, arguments);

            this.init.apply(this, arguments);
        },

        _init: function (attributes, options) {
            this.options = options || {};
        },

        init: function (attributes, options) {
            // Override this at will
        },

        parse: function (response) {
            if (response instanceof app.SocketResponse) {
                return this.postParse(response.data || {});
            }
            return this.postParse(response);
        },

        postParse: function (data) {
            return data;
        }
    });

    Backbone.ModernCollection = Backbone.Collection.extend({
        initialize: function (models, options) {
            this._init.apply(this, arguments);

            this.init.apply(this, arguments);
        },

        _init: function (models, options) {
            this.options = options || {};
        },

        init: function (models, options) {
            // Override this at will
        },

        parse: function (response) {
            if (response instanceof app.SocketResponse) {
                return this.postParse(response.data || {});
            }
            return this.postParse(response);
        },

        postParse: function (data) {
            return data;
        }
    });
})(
    Backbone,
    window.webapp
);
