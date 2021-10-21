#include <gmock/gmock.h>

#include <iostream>

#include <entt/entt.hpp>

struct color_t {};
struct level_t {};
struct status_t {
    bool status;
};

void draw(entt::registry& registry, std::ostream& out) {
    const auto &light_bulb_view = registry.view<status_t>();
    light_bulb_view.each([&light_bulb_view, &out](auto entity, auto &status) {
        out << std::boolalpha << light_bulb_view.get<status_t>(entity).status << std::endl;
    });
    out << "-----" << std::endl;
}

void switchStatus(entt::registry &registry, bool status_to_apply) {
    const auto &light_bulb_view = registry.view<status_t>();
    light_bulb_view.each([&](auto entity, auto &status) {
        status_t &status_ = light_bulb_view.get<status_t>(entity);
        status_.status = status_to_apply;
    });
}

TEST(light_management, entt)
{
    entt::registry registry;
    
    const auto color_light_bulb = registry.create();
    const auto dimmerable_light_bulb = registry.create();
    const auto light_bulb = registry.create();

    registry.emplace<color_t>(color_light_bulb);
    registry.emplace<level_t>(color_light_bulb);
    registry.emplace<status_t>(color_light_bulb, true);

    registry.emplace<level_t>(dimmerable_light_bulb);
    registry.emplace<status_t>(dimmerable_light_bulb, true);

    registry.emplace<status_t>(light_bulb, true);

    std::ostringstream oss;
    draw(registry, oss);
    switchStatus(registry, false);
    draw(registry, oss);
    ASSERT_EQ("true\ntrue\ntrue\n-----\nfalse\nfalse\nfalse\n-----\n", oss.str());
}
