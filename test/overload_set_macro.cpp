#include <cstdio>

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1170r0.html

#define FWD(x) static_cast<decltype(x)&&>(x)
#define RETURNS(expr)                                                          \
  noexcept(noexcept(expr))->decltype(expr)                                     \
  {                                                                            \
    return expr;                                                               \
  }
#define OVERLOADS_OF(name) [&](auto&&... args) RETURNS(name(FWD(args)...))

void
f(int)
{
  std::printf("Hello ");
}
void
f(double)
{
  std::printf("World!\n");
}

int
main()
{
  auto o = OVERLOADS_OF(f);
  o(1);
  o(1.);
}
