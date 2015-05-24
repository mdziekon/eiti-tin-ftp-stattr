/*global webapp, $*/
'use strict';

window.webapp = {
    Models: {},
    Collections: {},
    Views: {},
    Routers: {},

    router: undefined,
    section: undefined,

    connection: undefined,
    apiURL: "http://localhost:7080/bd2/api/",

    init: function () {
        var router = new webapp.Routers.Main();
        window.webapp.router  = router;

        window.webapp.connection = new window.webapp.SocketConnection({
            attributes: {
                host: "localhost",
                port: "3338"
            }
        });

        // All navigation that is relative should be passed through the navigate
        // method, to be processed by the router. If the link has a data-bypass
        // attribute, bypass the delegation completely.
        $(document).on("click", "a:not([data-bypass])", function (evt) {
            var $this = $(this);

            // Get the anchor href and protcol
            var href = $this.attr("href");
            var protocol = this.protocol + "//";

            // Ensure the protocol is not part of URL, meaning its relative.
            if (href && href !== "#" && href.slice(0, protocol.length) !== protocol) {
                // Stop the default event to ensure the link will not cause a page refresh
                evt.preventDefault();

                // This uses the default router defined above, and not any routers
                // that may be placed in modules. To have this work globally (at the
                // cost of losing all route events) you can change the following line
                // to: Backbone.history.navigate(href, true);
                router.navigate(href, true);
            }
        });

        Backbone.history.start({
            pushState: false
        });
    },

    getSection: function () {
        return this.section;
    },

    changeSection: function (sectionName) {
        if (this.section === sectionName) {
            return false;
        }

        this.section = sectionName;

        var $sections = $(".sections > div");
        var $sectionEl = $sections.filter("[data-section-name='" + this.section + "']");
        $sections.toggleClass("hidden", true);
        $sectionEl.toggleClass("hidden", false);

        var $nav = $("#side-menu");
        var $sectionNav = $nav.find("[href^='" + this.section + "']");

        $nav.find(".active").removeClass("active");
        if ($sectionNav.length) {
            $sectionNav.addClass("active");
        }

        return true;
    },

    getAPIUrl: function (url) {
        return this.apiURL + (this.apiURL[this.apiURL.length - 1] === "/" ? "" : "/") + url;
    },

    getJQElement: function (el) {
        return (!(el instanceof $)) ? $(el) : el;
    },

    showError: function ($container, message) {
        $container = webapp.getJQElement($container);

        $container.text(message);
        $container.show();
    },

    hideError: function ($container) {
        $container = webapp.getJQElement($container);

        $container.hide();
    },

    buttonToggleLoading: function ($button, enable) {
        if (enable && $button.data("is-loading") === "true") {
            return;
        }
        if (!enable && $button.data("is-loading") !== "true") {
            return;
        }

        if (enable) {
            $button.data("loading-old-classes", $button.attr("class"));
            $button.data("is-loading", "true");

            $button.button("load");
            $button.addClass("disabled");
        } else {
            $button.attr("class", $button.data("loading-old-classes"));
            $button.button("reset");

            $.removeData($button, "loading-old-classes");
            $.removeData($button, "is-loading");
        }
    },

    flashButton: function ($button, state, durance) {
        var dfd = new $.Deferred();

        webapp.buttonToggleLoading($button, false);

        $button.button(state);
        $button.data("flashing-old-classes", $button.attr("class"));
        $button.addClass("disabled");

        if (state === "complete" || state === "error") {
            $button.removeClass("btn-default btn-primary btn-success btn-info btn-warning btn-danger btn-link");
        }

        if (state === "complete") {
            $button.addClass("btn-success");
        } else if (state === "error") {
            $button.addClass("btn-danger");
        }

        if (durance >= 0) {
            window.setTimeout(function () {
                $button.attr("class", $button.data("flashing-old-classes"));
                $button.button("reset");

                $.removeData($button, "flashing-old-classes");

                dfd.resolve();
            }, durance);
        } else {
            $button.attr("class", $button.data("flashing-old-classes"));

            $.removeData($button, "flashing-old-classes");

            dfd.resolve();
        }

        return dfd.promise();
    }
};

$(document).ready(function () {
    webapp.init();
});
