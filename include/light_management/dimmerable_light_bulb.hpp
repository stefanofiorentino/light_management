#pragma once

#include <iostream>
#include "components/light_concept.hpp"

struct dimmerable_light_bulb_t final : light_concept_t
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
