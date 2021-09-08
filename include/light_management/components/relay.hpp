#pragma once

struct relay_t final
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return true;
    }
};
