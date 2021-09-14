#include <iostream>

#include <entt/entt.hpp>

struct color_t {};
struct level_t {};
struct status_t {
    bool status;
};

void draw(entt::registry& registry) {
    auto &light_bulb_view = registry.view<status_t>();
    light_bulb_view.each([&light_bulb_view](auto entity, auto &status) {
        std::cerr << std::boolalpha << light_bulb_view.get<status_t>(entity).status << std::endl;
    });
    std::cerr << "-----" << std::endl;
}

void switchStatus(entt::registry &registry, bool status_to_apply) {
    auto &light_bulb_view = registry.view<status_t>();
    light_bulb_view.each([&](auto entity, auto &status) {
        status_t &status_ = light_bulb_view.get<status_t>(entity);
        status_.status = status_to_apply;
    });
}

int main(int, char **)
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

    draw(registry);
    switchStatus(registry, false);
    draw(registry);

    return EXIT_SUCCESS;
}
