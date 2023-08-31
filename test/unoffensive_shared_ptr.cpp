#include <gmock/gmock.h>

#include <concepts>
#include <limits>
#include <memory>
#include <numeric>

class unoffensive_shared_ptr
{
  std::shared_ptr<int> sp;

  int* get_raw_ptr_or_default() const
  {
    if (!sp) {
      static auto local_sp =
        std::make_shared<int>(std::numeric_limits<int>::min());
      return local_sp.get();
    }
    return sp.get();
  }

public:
  explicit unoffensive_shared_ptr(std::shared_ptr<int> const& sp)
    : sp(sp)
  {
  }
  int* get() const noexcept { return get_raw_ptr_or_default(); }

  int& operator*() const noexcept { return *get_raw_ptr_or_default(); }

  int* operator->() const noexcept { return get_raw_ptr_or_default(); }
};

TEST(unoffensive_shared_ptr, pointer_to_one)
{
  std::shared_ptr<int> sp = std::make_shared<int>(1);
  unoffensive_shared_ptr usp{ sp };
  EXPECT_EQ(1, *usp.get());
  EXPECT_EQ(1, usp.operator*());
  EXPECT_EQ(1, *usp.operator->());
}

TEST(unoffensive_shared_ptr, return_default_if_nullptr)
{
  std::shared_ptr<int> sp = nullptr;
  unoffensive_shared_ptr usp{ sp };
  EXPECT_EQ(std::numeric_limits<int>::min(), *usp.get());
  EXPECT_EQ(std::numeric_limits<int>::min(), usp.operator*());
  EXPECT_EQ(std::numeric_limits<int>::min(), *usp.operator->());
}
