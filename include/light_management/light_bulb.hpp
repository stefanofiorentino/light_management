#pragma once

#include <iostream>

struct light_bulb_t final 
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return false;
    }
};
