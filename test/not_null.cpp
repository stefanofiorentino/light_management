#include <gmock/gmock.h>

struct base {
  virtual ~base() = default;
  virtual bool foo() const noexcept { return true; }
};

template <typename T> struct not_null {
  explicit not_null(T t) : pointee(t) {
    static_assert(std::is_pointer_v<T>,
                  "not_null accepts only pointer types as template parameter");
  }

  explicit not_null(std::nullptr_t) = delete;

  T operator->() { return pointee; }

private:
  T pointee;
};

TEST(not_null, simple) {
  base b;
  not_null<base *> pb(&b);
  ASSERT_TRUE(pb->foo());
}
