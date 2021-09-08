#pragma once

#include "non_copyable.hpp"
#include "dimmerable_light_bulb.hpp"

#include "components/color.hpp"

struct color_light_bulb_t final : light_management::non_copyable_t
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        bool rc = color.reset();
        return rc && dimmer.reset();
    }

private:
    color_t color;
    dimmerable_light_bulb_t dimmer;
};
