#pragma once

#include "light_bulb.hpp"

#include "components/level.hpp"

struct dimmerable_light_bulb_t final 
{
    void setFullScale(float fullScale_)
    {
        fullScale = fullScale_;
    }
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        bool rc = level.reset();
        return rc && light_bulb.reset();
    }

private:
    float fullScale;
    level_t level;
    light_bulb_t light_bulb;
};
