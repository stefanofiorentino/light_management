#pragma once

#include "non_copyable.hpp"
#include "light_bulb.hpp"

#include "components/level.hpp"

struct dimmerable_light_bulb_t final : light_management::non_copyable_t
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        bool rc = level.reset();
        return rc && light_bulb.reset();
    }

private:
    level_t level;
    light_bulb_t light_bulb;
};
