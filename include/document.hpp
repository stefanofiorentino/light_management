#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "light_management/type_traits.hpp"

template <typename T>
void draw(const T& x, std::ostream& out, size_t position);

template <typename T>
void do_switch(T& x, bool status);

class object_t {
public: 
    template <typename T>
    object_t(T x) : self_(std::make_shared<model_t<T>>(std::move(x))) {}
    
    friend void draw(const object_t&x, std::ostream& out, size_t position)
    { x.self_->draw(out, position); }

    friend void do_switch(object_t&x, bool status)
    { x.self_->do_switch(status); }
private: 
    struct concept_t
    {
        virtual ~concept_t() = default;
        virtual void draw(std::ostream& out, size_t position) const = 0;
        virtual void do_switch(bool) = 0;
    };
    template <typename T>
    struct model_t final : concept_t
    {
        model_t(T x) : data_(std::move(x)){}
        void draw(std::ostream& out, size_t position) const override
        {
          if constexpr (has_void_draw_v<T>) {
            ::draw(data_, out, position);
          }
        }
        void do_switch(bool status) override
        {
            if constexpr (has_void_do_switch_v<T>) {
                ::do_switch(data_, status);
            }
        }
        T data_;
    };
    std::shared_ptr<concept_t> self_;
};

using collection_t = std::vector<object_t>;

template <typename T>
void draw(const T& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << x << "\n";
}

template <>
void draw(const collection_t& x, std::ostream& out, size_t position)
{
    for (const auto& e: x) draw(e, out, position + 2);
}

template <typename T>
void do_switch(T& x, bool status)
{
    x.do_switch(status);
}

template <>
void do_switch(collection_t& x, bool status)
{
    for (auto& e: x) do_switch(e, status);
}
