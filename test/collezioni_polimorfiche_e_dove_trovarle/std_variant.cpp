#include <gmock/gmock.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/color_dimmable_light_bulb.hpp>

template <typename T>
using collection_t = std::vector<T>;

using light_concept_t = std::variant<light_bulb_t, dimmable_light_bulb_t, color_dimmable_light_bulb_t>;

void dump(collection_t<light_concept_t> & collection, std::ostream& os)
{
    os << "<document>\n";
    std::for_each(collection.begin(), collection.end(), [&os](light_concept_t& light_){
        std::visit([&os](auto&& light) {
            if constexpr (has_void_draw_v<decltype(light)>)
            {
                light.draw(os);
            }
        }, light_);
    });
    os << "</document>\n";
}

TEST(light_management, std_variant)
{
    collection_t<light_concept_t> c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    
    std::ostringstream oss;
    dump(c, oss);
    ASSERT_EQ("<document>\n<light_bulb_t/>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n</document>\n", oss.str());
}
