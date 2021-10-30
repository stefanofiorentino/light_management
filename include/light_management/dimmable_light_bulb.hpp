#pragma once

#include <iostream>

struct dimmable_light_bulb_t final
{
    void draw(std::ostream &oss)
    {
        oss << "<dimmable_light_bulb_t/>\n";
    }
};
