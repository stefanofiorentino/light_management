#include <gmock/gmock.h>

#include <iostream>
#include <memory>

#include <light_management/type_traits/has_int_value.hpp>

// https://www.youtube.com/watch?v=0I0FD3N5cgM
struct anything {
  anything() = default;
  anything(anything const &rhs) : handle_(rhs.handle_->clone()) {}
  anything &operator=(anything const &rhs) {
    anything temp(rhs);
    std::swap(temp, *this);
    return *this;
  }
  template <typename T>
  anything(T t)
      : handle_(new handle<typename std::remove_reference<T>::type>(
            std::forward<T>(t))) {}
  template <typename T> anything &operator=(T t) {
    anything temp(std::forward<T>(t));
    std::swap(temp, *this);
    return *this;
  }

  // Public interface
  int value() const {
    assert(handle_);
    return handle_->value();
  }

private:
  struct handle_base {
    virtual ~handle_base() = default;
    virtual handle_base *clone() const = 0;
    virtual int value() const = 0;
  };

  template <typename T> struct handle : handle_base {
    handle(T value) : value_(std::move(value)) {}
    virtual handle_base *clone() const { return new handle(value_); }
    virtual int value() const { return value_.value(); };
    T value_;
  };

  std::unique_ptr<handle_base> handle_;
};

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
  visit([](auto const &v, auto &&rc) {
    if
      constexpr(has_int_value_v<std::remove_cv_t<decltype(v)>>) {
        rc = v.value();
      }
  }, f, rc);
  ASSERT_EQ(EXIT_SUCCESS, rc);
}

struct foo_without_value {};

TEST(pragmatic_type_erasure, visit_with_constexpr_checks_failing) {
  static_assert(!has_int_value_v<foo_without_value>, "Assert failed!");
}
