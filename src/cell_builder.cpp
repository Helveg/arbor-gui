#include "cell_builder.hpp"

#include <sstream>
#include "utils.hpp"

cell_builder::cell_builder(): tree{}, morph{}, pwlin{morph}, labels{}, provider{morph, labels} {};
cell_builder::cell_builder(const arb::segment_tree& t): tree{t}, morph{tree}, pwlin{morph}, labels{}, provider{morph, labels} {};

std::vector<arb::msegment> cell_builder::make_segments(const arb::region& region) {
    auto concrete = thingify(region, provider);
    return pwlin.all_segments(concrete);
}

std::vector<glm::vec3> cell_builder::make_points(const arb::locset& locset) {
    auto concrete = thingify(locset, provider);
    std::vector<glm::vec3> points;
    points.resize(concrete.size());
    std::transform(concrete.begin(), concrete.end(), points.begin(),
                   [&](const auto& loc) {
                       auto p = pwlin.at(loc);
                       return glm::vec3{p.x, p.y, p.z};});
    std::stringstream ss; ss << locset;
    log_debug("Made locset {} markers: {} points", ss.str(), points.size());
    return points;
}

arb::cable_cell cell_builder::make_cell() { return {morph, labels}; }
