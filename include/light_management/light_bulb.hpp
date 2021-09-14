#pragma once

#include <iostream>
#include "components/light_concept.hpp"

struct light_bulb_t final : light_concept_t
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return false;
    }
};
