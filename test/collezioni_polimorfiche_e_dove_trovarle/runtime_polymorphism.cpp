#include <gmock/gmock.h>

#include <document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg
#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/color_dimmable_light_bulb.hpp>
#include <light_management/room.hpp>

TEST(runtime_polymorphism, draw_plain_plant) {
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());

    std::ostringstream oss;
    draw(c, oss, 0);
    ASSERT_EQ("<light_bulb_t>false</light_bulb_t>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n", oss.str());
}

TEST(runtime_polymorphism, draw_nested_plant) {
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    room_t r;
    r.emplace_back(light_bulb_t());
    c.emplace_back(std::move(r));

    std::ostringstream oss;
    draw(c, oss, 0);
    ASSERT_EQ("<light_bulb_t>false</light_bulb_t>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n<room_t>\n<light_bulb_t>false</light_bulb_t>\n</room_t>\n", oss.str());
}

TEST(runtime_polymorphism, do_switch) {
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());

    std::ostringstream oss;
    do_switch(c, true);
    draw(c, oss, 0);
    ASSERT_EQ("<light_bulb_t>true</light_bulb_t>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n", oss.str());
}
