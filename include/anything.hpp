#pragma once

#include <utility>
#include <cassert>
#include <memory>

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
    handle_base *clone() const override { return new handle(value_); }
    int value() const override { return value_.value(); };
    T value_;
  };

  std::unique_ptr<handle_base> handle_;
};

using collection_t = std::vector<anything>;
typedef decltype(*(std::declval<collection_t>().begin())) collection_value_t;
