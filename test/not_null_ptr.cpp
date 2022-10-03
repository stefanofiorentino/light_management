#include <gmock/gmock.h>

#include <stdexcept>

struct base {
  virtual ~base() = default;
  virtual bool foo() const noexcept { return true; }
};

template <typename T> struct not_null_ptr {
  explicit not_null_ptr(T *t) : pointee(t) {}

  explicit not_null_ptr(std::nullptr_t) = delete;

  T *operator->() const {
    if (!pointee)
      throw std::runtime_error("");
    return pointee;
  }

private:
  T *pointee;
};

TEST(not_null_ptr, whenSimpleThenWorks) {
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(pb->foo());
}

static base *getPtr() { return nullptr; }

TEST(not_null_ptr, whenOwnsANullptrThenThrows) {
  not_null_ptr<base> pb(getPtr());
  ASSERT_THROW(pb->foo(), std::runtime_error);
}
