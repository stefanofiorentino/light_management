#pragma once

#include <iostream>

struct light_bulb_t final 
{
    void draw(std::ostream &oss) const
    {
        oss << "<light_bulb_t/>\n";
    }
};
