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

template <typename F, typename... Args> void visit(F f, Args &&... args) {
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

TEST(pragmatic_type_erasure, visit_a_collection) {
  collection_t c;
  c.push_back(foo());
  c.push_back(bar());

  std::for_each(c.begin(), c.end(), [](collection_value_t &&v) { v.value(); });
}

struct none {
  int value() const { return EXIT_FAILURE; }
};

TEST(pragmatic_type_erasure, collection_general_) {
  collection_t c;
  c.push_back(foo());
  c.push_back(bar());

  ASSERT_TRUE(std::all_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_SUCCESS; }));
  ASSERT_TRUE(std::none_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_FAILURE; }));
  
  c.push_back(none());
  ASSERT_FALSE(std::all_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_SUCCESS; }));
  ASSERT_FALSE(std::none_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_FAILURE; }));
  ASSERT_TRUE(std::any_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_FAILURE; }));
  ASSERT_TRUE(std::any_of(c.begin(), c.end(), [](collection_value_t &&v) { return v.value() == EXIT_SUCCESS; }));
}
