#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "light_management/type_traits.hpp"

template <typename T>
void draw(const T& x, std::ostream& out, size_t position);

class object_t {
public: 
    template <typename T>
    object_t(T x) : self_(std::make_shared<model_t<T>>(std::move(x))) {}
    
    friend void draw(const object_t&x, std::ostream& out, size_t position)
    { x.self_->draw(out, position); }
private: 
    struct concept_t
    {
        virtual ~concept_t() = default;
        virtual void draw(std::ostream& out, size_t position) const = 0;
    };
    template <typename T>
    struct model_t final : concept_t
    {
        model_t(T x) : data_(std::move(x)){}
        void draw(std::ostream& out, size_t position) const override
        {
            ::draw(data_, out, position); 
        }
        T data_;
    };
    std::shared_ptr<const concept_t> self_;
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
    out << std::string(position, ' ') << "<document>\n" ;
    for (const auto& e: x) draw(e, out, position + 2);
    out << std::string(position, ' ') << "</document>\n" ;
}
