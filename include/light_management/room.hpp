#pragma once

#include <iostream>
#include <vector>

#include "light_bulb.hpp"
#include <document.hpp>

struct room_t final
{
  template<typename T>
  void emplace_back(T&& t)
  {
    children_.emplace_back(std::forward<T>(t));
  }

  void draw(std::ostream& oss) const
  {
    if (children_.empty()) {
      oss << "<room_t/>\n";
      return;
    }
    oss << "<room_t>\n";
    ::draw(children_, oss, 0);
    oss << "</room_t>\n";
  }

  collection_t children_;
};

template<>
void
draw(const room_t& room, std::ostream& out, size_t /* position */)
{
  room.draw(out);
}
