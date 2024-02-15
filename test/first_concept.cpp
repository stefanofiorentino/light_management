#include <concepts>

template<typename T>
concept has_draw_member_function = requires(T v)
{
  {
    v.draw()
    } -> std::convertible_to<void>;
};

int
main()
{
  struct invalid
  {};
  static_assert(!has_draw_member_function<invalid>);

  struct valid
  {
    void draw() {}
  };
  static_assert(has_draw_member_function<valid>);
}
