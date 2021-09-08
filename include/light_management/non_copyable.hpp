#pragma once

namespace light_management
{
    struct non_copyable_t
    {
        non_copyable_t() = default;
        non_copyable_t(non_copyable_t const &) = delete;
        non_copyable_t &operator=(non_copyable_t const &) = delete;
        non_copyable_t(non_copyable_t &&) = default;
        non_copyable_t &operator=(non_copyable_t &&) = default;
        virtual ~non_copyable_t() = default;
    };
}