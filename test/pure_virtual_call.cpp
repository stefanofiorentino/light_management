#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct base {
  virtual ~base() = default;
  virtual void foo() const = 0;
};

struct derived final : base {
  void foo() const {}
};

TEST(pure_virtual_call, simple) {
  base *b;
  {
    derived d;
    b = &d;
  }
  ASSERT_DEATH_IF_SUPPORTED({ b->foo(); }, "");
}