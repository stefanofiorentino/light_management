#pragma once

struct level_t final
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return true;
    }
};
