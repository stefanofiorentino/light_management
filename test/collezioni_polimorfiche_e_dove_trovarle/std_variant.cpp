#include <gmock/gmock.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmerable_light_bulb.hpp>
#include <light_management/color_light_bulb.hpp>

template <typename T>
using collection = std::vector<T>;

using light_concept_t = std::variant<light_bulb_t, dimmerable_light_bulb_t, color_light_bulb_t>;

TEST(light_management, std_variant)
{
    collection<light_concept_t> c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmerable_light_bulb_t());
    c.emplace_back(color_light_bulb_t());
    
    std::for_each(c.begin(), c.end(), [](light_concept_t& light_){
        std::visit([](auto&& light) {
            if constexpr (has_bool_reset_v<decltype(light)>)
            {
                light.reset();
            }
        }, light_);
    });
}
