#include <gmock/gmock.h>

#include <iostream>
#include <memory>

#include <light_management/type_traits/has_int_value.hpp>

#include <anything.hpp>

struct foo {
  int value() const { return EXIT_SUCCESS; }
};
struct bar {
  int value() const { return EXIT_SUCCESS; }
};

int value_of() {
  if (anything(foo()).value() == anything(bar()).value()) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

TEST(pragmatic_type_erasure, anything) { ASSERT_EQ(EXIT_SUCCESS, value_of()); }

template <typename F, typename... Args> 
void visit(F f, Args &&... args) {
  f(args...);
}

TEST(pragmatic_type_erasure, visit) {
  anything f = foo();
  anything b = bar();
  ASSERT_EQ(EXIT_SUCCESS, f.value());
  ASSERT_EQ(EXIT_SUCCESS, b.value());

  auto rc = EXIT_FAILURE;
  visit([](auto const &v, auto &&rc) { rc = v.value(); }, f, rc);
  ASSERT_EQ(EXIT_SUCCESS, rc);
}

TEST(pragmatic_type_erasure, visit_with_constexpr_checks_success) {
  anything f = foo();
  anything b = bar();
  ASSERT_EQ(EXIT_SUCCESS, f.value());
  ASSERT_EQ(EXIT_SUCCESS, b.value());

  auto rc = EXIT_FAILURE;
  visit(
      [](auto const &v, auto &&rc) {
        if constexpr (has_int_value_v<std::remove_cv_t<decltype(v)>>) {
          rc = v.value();
        }
      },
      f, rc);
  ASSERT_EQ(EXIT_SUCCESS, rc);
}

struct foo_without_value {};

TEST(pragmatic_type_erasure, visit_with_constexpr_checks_failing) {
  static_assert(!has_int_value_v<foo_without_value>, "Assert failed!");
}
