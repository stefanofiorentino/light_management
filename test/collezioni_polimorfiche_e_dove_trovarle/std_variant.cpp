#include <gmock/gmock.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/color_light_bulb.hpp>

template <typename T>
using collection_t = std::vector<T>;

using light_concept_t = std::variant<light_bulb_t, dimmable_light_bulb_t, color_dimmable_light_bulb_t>;

TEST(light_management, std_variant)
{
    collection_t<light_concept_t> c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    
    std::ostringstream oss;
    std::for_each(c.begin(), c.end(), [&oss](light_concept_t& light_){
        std::visit([&oss](auto&& light) {
            if constexpr (has_void_draw_v<decltype(light)>)
            {
                light.draw(oss);
            }
        }, light_);
    });

    ASSERT_EQ("<light_bulb_t/>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n", oss.str());
}
