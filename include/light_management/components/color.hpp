#pragma once

struct color_t final
{
    bool reset() const
    {
        std::puts(__FUNCSIG__);
        return true;
    }
};
