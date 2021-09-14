#pragma once

#include <iostream>

struct dimmerable_light_bulb_t
{
    void setFullScale(float fullScale_)
    {
        std::puts(__FUNCSIG__);
    }
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return false;
    }
};
