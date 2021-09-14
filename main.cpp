#include <iostream>
#include <vector>
#include <algorithm>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmerable_light_bulb.hpp>
#include <light_management/color_light_bulb.hpp>

#include <light_management/document.hpp>

struct my_class_t {};

void draw(const my_class_t&, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "my_class_t" << "\n";
}

void test_helper(std::ostream& out)
{    
    document_t document;
    document.reserve(5);

    document.emplace_back(0);
    document.emplace_back(std::string("Hello!"));
    document.emplace_back(document);
    document.emplace_back(my_class_t());
    document.emplace_back(4);

    std::reverse(std::begin(document), std::end(document));
    draw(document, out, 0);
}


int main()
{
    test_helper(std::cout);
}
