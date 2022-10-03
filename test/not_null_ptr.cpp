#include <gmock/gmock.h>

struct base {
  virtual ~base() = default;
  virtual bool foo() const noexcept { return true; }
};

template <typename T> struct not_null_ptr {
  explicit not_null_ptr(T *t) : pointee(t) {}

  explicit not_null_ptr(std::nullptr_t) = delete;

  T *operator->() { return pointee; }

private:
  T *pointee;
};

TEST(not_null_ptr, simple) {
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(pb->foo());
}
