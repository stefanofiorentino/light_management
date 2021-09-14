#pragma once

#include <iostream>
#include "components/light_concept.hpp"

struct color_light_bulb_t final : light_concept_t
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
