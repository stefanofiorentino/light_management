#pragma once

#include <iostream>

struct color_dimmable_light_bulb_t final
{
    void draw(std::ostream &oss)
    {
        oss << "<color_dimmable_light_bulb_t/>\n";
    }
};
