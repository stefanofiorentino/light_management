#pragma once

#include <iostream>

struct light_concept_t
{
    virtual ~light_concept_t() = default;
    virtual bool reset() const {
        std::puts("do nothing");
        return false;
    }
};
