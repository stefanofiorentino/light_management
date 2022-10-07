#include <gmock/gmock.h>

#include <scoped_allocator>
#include <string>
#include <utility>

#include "visitor_guide_allocators/constants.h"
#include "visitor_guide_allocators/statically_allocated_arena.hpp"
#include "visitor_guide_allocators/statically_allocated_arena_allocator.h"

using Arena1024 = Arena<1024>;
using ArenaAllocator1024 = ArenaAllocator<void, Arena1024>;

template <typename T>
using SA1024 = std::scoped_allocator_adaptor<ArenaAllocator<T, Arena1024>>;

using astring = std::basic_string<char, std::char_traits<char>, SA1024<char>>;

astring make_astring(std::string const &rhs,
                     ArenaAllocator1024 &arenaAllocator) {
  return astring(rhs, arenaAllocator);
}

TEST(statically_allocated_string, whenStdStringIsUsedThenItWorks) {
  Arena1024 arena;
  ArenaAllocator1024 arenaAllocator(&arena);
  auto str =
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, str.c_str());
}
