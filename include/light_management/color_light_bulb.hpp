#pragma once

#include <iostream>

struct color_light_bulb_t final
{
    void setColor()
    {
        std::puts(__FUNCSIG__);
    }
    void setFullScale()
    {
        std::puts(__FUNCSIG__);
    }
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return false;
    }
};
