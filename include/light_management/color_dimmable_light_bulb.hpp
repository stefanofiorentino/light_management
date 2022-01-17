#pragma once

#include <iostream>
#include <document.hpp>

struct color_dimmable_light_bulb_t final
{
    void draw(std::ostream &oss) const
    {
        oss << "<color_dimmable_light_bulb_t/>\n";
    }
};

template <>
void draw(const color_dimmable_light_bulb_t &light, std::ostream &out, size_t /*position*/) {
  light.draw(out);
}
