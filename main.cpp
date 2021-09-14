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

    return EXIT_SUCCESS;
}
