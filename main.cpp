#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmerable_light_bulb.hpp>
#include <light_management/color_light_bulb.hpp>

struct unsupported_light_t
{
    bool setFullScale(float fullScale_) {}
    void reset() { return; }
};

struct not_light_but_supported_t
{
    void setColor() {}
    void setFullScale() {}
    bool reset() { return true; }
};

template <typename T>
using collection = std::vector<T>;

using light_concept_t = std::variant<light_bulb_t, dimmerable_light_bulb_t, color_light_bulb_t>;

int main(int, char **)
{
    // unsupported
    static_assert(!is_color_light_bulb_v<void>, "`void` should not be a supported color light");
    static_assert(!is_color_light_bulb_v<int>, "`int` should not be a supported color light");
    static_assert(!is_color_light_bulb_v<unsupported_light_t>, "`unsupported_light_t` should not be a supported color light");
    static_assert(!has_bool_reset_v<unsupported_light_t>, "`unsupported_light_t` should not be a supported color light");
    static_assert(!has_void_setFullScale_v<unsupported_light_t>, "`unsupported_light_t` should not be a supported color light");
    static_assert(!has_void_setColor_v<unsupported_light_t>, "`unsupported_light_t` should not be a supported color light");

    // supported
    static_assert(is_color_light_bulb_v<not_light_but_supported_t>, "`not_light_but_supported_t` should be a supported color light");
    static_assert(is_color_light_bulb_v<color_light_bulb_t>, "`color_light_bulb_t` should be a supported color light");
    static_assert(has_void_setColor_v<color_light_bulb_t>, "`color_light_bulb_t` should be a supported color light");

    collection<light_concept_t> c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmerable_light_bulb_t());
    c.emplace_back(color_light_bulb_t());
    
    std::for_each(c.begin(), c.end(), [](light_concept_t& light_){
        std::visit([](auto&& light) {
            light.reset();
        }, light_);
    });

    return EXIT_SUCCESS;
}
