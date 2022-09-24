// https://rawgit.com/google/cxx-std-draft/allocator-paper/allocator_example_usage.html

#include <gmock/gmock.h>

#include <scoped_allocator>
#include <string>
#include <utility>

#include "visitor_guide_allocators/arena.h"
#include "visitor_guide_allocators/arena_allocator.h"
#include "visitor_guide_allocators/constants.h"

template <typename T>
using SA = std::scoped_allocator_adaptor<ArenaAllocator<T>>;
using astring = std::basic_string<char, std::char_traits<char>, SA<char>>;

astring make_astring(std::string const &rhs,
                     ArenaAllocator<void> &arenaAllocator) {
  return astring(rhs, arenaAllocator);
}

TEST(visitor_guide_allocators, whenStdStringIsUsedThenItWorks) {
  Arena arena(1024);
  ArenaAllocator<void> arenaAllocator(&arena);
  auto s2 =
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators, whenCharPointerIsUsedThenItWorks) {
  Arena arena(1024);
  ArenaAllocator<void> arenaAllocator(&arena);
  auto s2 = make_astring(experimental::constants::VERY_LONG_STRING_C_STR,
                         arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators, whenExceedingSizeThenAnExceptionIsThrown) {
  Arena arena(100);
  ArenaAllocator<void> arenaAllocator(&arena);
  ASSERT_THROW(
      make_astring(experimental::constants::VERY_LONG_STRING_CAUSING_EXCEPTION,
                   arenaAllocator),
      std::bad_alloc);
}

TEST(visitor_guide_allocators, whenDestroyThenSpaceIsFreed) {
  Arena arena(100);
  ArenaAllocator<void> arenaAllocator(&arena);
  auto s0 =
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  auto s1 =
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  auto s2 =
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  
  s1.clear();
  s1.shrink_to_fit();
  
  ASSERT_NO_THROW(
      make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator));
}
