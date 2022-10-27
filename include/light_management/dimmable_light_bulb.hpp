#pragma once

#include <document.hpp>
#include <iostream>

struct dimmable_light_bulb_t final
{
  void draw(std::ostream& oss) const { oss << "<dimmable_light_bulb_t/>\n"; }
};

template<>
void
draw(const dimmable_light_bulb_t& light, std::ostream& out, size_t /*position*/)
{
  light.draw(out);
}
