#include <gmock/gmock.h>

#include <concepts>
#include <limits>
#include <memory>

template<typename T>
class unoffensive_shared_ptr
{
  std::shared_ptr<T> sp;

  T* get_raw_ptr_or_default() const
  {
    if (!sp) {
      static T t{};
      return &t;
    }
    return sp.get();
  }

public:
  explicit unoffensive_shared_ptr(std::shared_ptr<T> const& sp)
    : sp(sp)
  {}
  T* get() const noexcept { return get_raw_ptr_or_default(); }

  T& operator*() const noexcept { return *get_raw_ptr_or_default(); }

  T* operator->() const noexcept { return get_raw_ptr_or_default(); }
};

TEST(unoffensive_shared_ptr, pointer_to_one)
{
  auto sp = std::make_shared<int>(1);
  unoffensive_shared_ptr<int> usp{ sp };
  EXPECT_EQ(1, *usp.get());
  EXPECT_EQ(1, usp.operator*());
  EXPECT_EQ(1, *usp.operator->());
}

TEST(unoffensive_shared_ptr, return_default_if_nullptr)
{
  std::shared_ptr<int> sp = nullptr;
  unoffensive_shared_ptr<int> usp{ sp };
  EXPECT_EQ(0, *usp.get());
  EXPECT_EQ(0, usp.operator*());
  EXPECT_EQ(0, *usp.operator->());
}

TEST(unoffensive_shared_ptr, pointer_to_char)
{
  auto sp = std::make_shared<char>('c');
  unoffensive_shared_ptr<char> usp{ sp };
  EXPECT_EQ('c', *usp.get());
  EXPECT_EQ('c', usp.operator*());
  EXPECT_EQ('c', *usp.operator->());
}

TEST(unoffensive_shared_ptr, pointer_to_valid_std__string)
{
  constexpr auto expected = "Hello, World!";
  auto sp = std::make_shared<std::string>(expected);
  unoffensive_shared_ptr<std::string> usp{ sp };
  EXPECT_EQ(expected, *usp.get());
  EXPECT_EQ(expected, usp.operator*());
  EXPECT_EQ(expected, *usp.operator->());
  EXPECT_EQ(13, usp->size());
}

TEST(unoffensive_shared_ptr, pointer_to_unvalid_std__string)
{
  std::shared_ptr<std::string> sp;
  unoffensive_shared_ptr<std::string> usp{ sp };
  EXPECT_EQ("", *usp.get());
  EXPECT_EQ("", usp.operator*());
  EXPECT_EQ("", *usp.operator->());
  EXPECT_EQ(0, usp->size());
}
