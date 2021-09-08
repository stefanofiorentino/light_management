#pragma once

#include "non_copyable.hpp"
#include "components/relay.hpp"

struct light_bulb_t final : light_management::non_copyable_t
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return relay.reset();
    }

private:
    relay_t relay;
};
