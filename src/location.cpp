#include "location.hpp"

loc_def::loc_def(const std::string_view n, const std::string_view d): name{n}, definition{d} {
    name.resize(512, '\0');
    definition.resize(512, '\0');
    change();
}

reg_def::reg_def(const std::string_view n, const std::string_view d): loc_def{n, d} {}
reg_def::reg_def(): loc_def{"", ""} {}

void reg_def::update() {
    if (state == def_state::erase) return;
    if (definition.empty() || !definition[0]) {
        data = {};
        empty();
    } else {
        try {
            data = arb::region{definition};
            good();
        } catch (const arb::label_parse_error& e) {
            data = {};
            std::string m = e.what();
            auto colon = m.find(':') + 1; colon = m.find(':', colon) + 1;
            error(m.substr(colon, m.size() - 1));
        }
    }
}

void reg_def::set_renderable(geometry& renderer, cell_builder& builder, renderable& render) {
    render.active = false;
    if (!data || (state != def_state::good)) return;
    log_info("Making frustrums for region {} '{}'", name, definition);
    try {
        auto points = builder.make_segments(data.value());
        render = renderer.make_region(points, render.color);
    } catch (arb::morphology_error& e) {
        error(e.what());
    }
}

ls_def::ls_def(const std::string_view n, const std::string_view d): loc_def{n, d} {}
ls_def::ls_def(): loc_def{"", ""} {}

void ls_def::update() {
    if (state == def_state::erase) return;
    if (definition.empty() || !definition[0]) {
        data = {};
        empty();
    } else {
        try {
            data = arb::locset{definition};
            good();
        } catch (const arb::label_parse_error& e) {
            data = {};
            std::string m = e.what();
            auto colon = m.find(':') + 1; colon = m.find(':', colon) + 1;
            error(m.substr(colon, m.size() - 1));
        }
    }
}

void ls_def::set_renderable(geometry& renderer, cell_builder& builder, renderable& render) {
    render.active = false;
    if (!data || (state != def_state::good)) return;
    log_info("Making markers for locset {} '{}'", name, definition);
    try {
        auto points = builder.make_points(data.value());
        render = renderer.make_marker(points, render.color);
    } catch (arb::morphology_error& e) {
        error(e.what());
    }
}
