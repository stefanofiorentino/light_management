#include <iostream>

#include <light_management/type_traits.hpp>

#include <light_management/color_light_bulb.hpp>

struct unsupported_light_t
{
    void reset() { return; }
};

struct not_light_but_supported_t
{
    bool reset() { return true; }
};

int main(int, char **)
{
    static_assert(!is_color_light_bulb_v<void>, "`void` should not be a supported color light");
    static_assert(!is_color_light_bulb_v<int>, "`int` should not be a supported color light");
    static_assert(!is_color_light_bulb_v<unsupported_light_t>, "`unsupported_light_t` should not be a supported color light");
    static_assert(is_color_light_bulb_v<not_light_but_supported_t>, "`not_light_but_supported_t` should be a supported color light");
    static_assert(is_color_light_bulb_v<color_light_bulb_t>, "`color_light_bulb_t` should be a supported color light");

    color_light_bulb_t color_light_bulb;
    return color_light_bulb.reset();
}
