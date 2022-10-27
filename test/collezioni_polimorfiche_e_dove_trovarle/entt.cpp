#include <gmock/gmock.h>

#include <iostream>

#include <entt/entt.hpp>

struct drawable_t
{};
struct color_t
{};
struct level_t
{};
struct status_t
{
  bool status;
};

void
draw(entt::registry& registry, std::ostream& os)
{
  os << "<document>\n";
  const auto& light_bulb_view = registry.view<drawable_t, status_t>();
  light_bulb_view.each([&light_bulb_view, &os](auto entity, auto&) {
    status_t const& status_ = light_bulb_view.get<status_t>(entity);
    os << "<light_bulb_t>";
    os << std::boolalpha << status_.status;
    os << "</light_bulb_t>\n";
  });
  os << "<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n</"
        "document>\n";
}

void
do_switch(entt::registry& registry, bool status_to_apply)
{
  const auto& light_bulb_view = registry.view<drawable_t, status_t>();
  light_bulb_view.each([&](auto entity, auto&) {
    status_t& status_ = light_bulb_view.get<status_t>(entity);
    status_.status = status_to_apply;
  });
}

TEST(entt, draw)
{
  entt::registry registry;

  const auto light_bulb = registry.create();
  registry.emplace<drawable_t>(light_bulb);
  registry.emplace<status_t>(light_bulb, false);

  const auto dimmable_light_bulb = registry.create();
  registry.emplace<drawable_t>(dimmable_light_bulb);
  registry.emplace<level_t>(dimmable_light_bulb);

  const auto color_light_bulb = registry.create();
  registry.emplace<drawable_t>(color_light_bulb);
  registry.emplace<color_t>(color_light_bulb);

  std::ostringstream oss;
  draw(registry, oss);
  ASSERT_EQ(
    "<document>\n<light_bulb_t>false</light_bulb_t>\n<dimmable_light_bulb_t/"
    ">\n<color_dimmable_light_bulb_t/>\n</document>\n",
    oss.str());
}

TEST(entt, do_switch)
{
  entt::registry registry;

  const auto light_bulb = registry.create();
  registry.emplace<drawable_t>(light_bulb);
  registry.emplace<status_t>(light_bulb, false);

  const auto dimmable_light_bulb = registry.create();
  registry.emplace<drawable_t>(dimmable_light_bulb);
  registry.emplace<level_t>(dimmable_light_bulb);

  const auto color_light_bulb = registry.create();
  registry.emplace<drawable_t>(color_light_bulb);
  registry.emplace<color_t>(color_light_bulb);

  std::ostringstream oss;
  do_switch(registry, true);
  draw(registry, oss);
  ASSERT_EQ(
    "<document>\n<light_bulb_t>true</light_bulb_t>\n<dimmable_light_bulb_t/"
    ">\n<color_dimmable_light_bulb_t/>\n</document>\n",
    oss.str());
}
