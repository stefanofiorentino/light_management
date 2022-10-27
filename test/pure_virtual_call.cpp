#include <gmock/gmock.h>

struct base
{
  virtual ~base() = default;
  virtual void foo() const = 0;
};

struct derived final : base
{
  void foo() const {}
};

[[noreturn]] void
onTerminate() noexcept
{
  std::_Exit(EXIT_SUCCESS);
}

const auto installed{ std::set_terminate(onTerminate) };

TEST(pure_virtual_call, simple)
{
  base* b;
  {
    derived d;
    b = &d;
  }
  b->foo();
}