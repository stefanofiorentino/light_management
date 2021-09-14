#pragma once

#include <iostream>

struct color_light_bulb_t
{
    void setFullScale()
    {
        std::puts(__FUNCSIG__);
    }
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return false;
    }
    void setColor()
    {
        std::puts(__FUNCSIG__);
    }
};
