#include <concepts>
#include <gmock/gmock.h>

template<typename T>
concept has_draw_member_function = requires(T v)
{
  {
    v.draw()
    } -> std::convertible_to<void>;
};

struct invalid
{};

struct valid
{
  void draw() {}
};

TEST(first_concept, simple){

  if constexpr (has_draw_member_function<invalid>) {
    FAIL();
  }

  if constexpr (!has_draw_member_function<valid>) {
    FAIL();
  }
}
