#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "light_management/type_traits.hpp"

template <typename T>
void draw(const T& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << x << "\n";
}

template <typename T>
void switchOff(const T&x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "switching OFF\n";
}

class object_t {
public: 
    template <typename T>
    object_t(T x) : self_(std::make_shared<model_t<T>>(std::move(x))) {}
    
    friend void draw(const object_t&x, std::ostream& out, size_t position)
    { x.self_->draw(out, position); }
    friend void switchOff(const object_t&x, std::ostream& out, size_t position)
    { x.self_->switchOff(out, position); }
private: 
    struct concept_t
    {
        virtual ~concept_t() = default;
        virtual void draw(std::ostream& out, size_t position) const = 0;
        virtual void switchOff(std::ostream& out, size_t position) const = 0;
    };
    template <typename T>
    struct model_t final : concept_t
    {
        model_t(T x) : data_(std::move(x)){}
        void draw(std::ostream& out, size_t position) const override
        {
            if constexpr (has_void_draw_v<T>)
            {
                ::draw(data_, out, position);
            }
        }
        void switchOff(std::ostream& out, size_t position) const override
        {
            if constexpr (has_void_switchOff_v<T>)
            {
                ::switchOff(data_, out, position);
            }
        }
        T data_;
    };
    std::shared_ptr<const concept_t> self_;
};

using document_t = std::vector<object_t>;

inline void draw(const document_t& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "<document>\n" ;
    for (const auto& e: x) draw(e, out, position + 2);
    out << std::string(position, ' ') << "</document>\n" ;
}

inline void switchOff(const document_t& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "switching ALL OFF\n";
    for (const auto& e: x) switchOff(e, out, position + 2);
}
