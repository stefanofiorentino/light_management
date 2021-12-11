#include <gmock/gmock.h>

#include <document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg
#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/color_dimmable_light_bulb.hpp>

template <>
void draw(const light_bulb_t &light, std::ostream &out, size_t /*position*/) {
  light.draw(out);
}

template <>
void draw(const color_dimmable_light_bulb_t &light, std::ostream &out, size_t /*position*/) {
  light.draw(out);
}

template <>
void draw(const dimmable_light_bulb_t &light, std::ostream &out, size_t /*position*/) {
  light.draw(out);
}

TEST(light_management, runtime_polymorphism) {
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    
    std::ostringstream oss;
    draw(c, oss, 0);
    ASSERT_EQ("<document>\n<light_bulb_t/>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n</document>\n", oss.str());
}
