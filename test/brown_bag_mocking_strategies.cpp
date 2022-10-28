#include <gmock/gmock.h>

#include <cstdio>
#include <iostream>

using ::testing::Return;

struct my_singleton_base
{
  virtual bool to_be_called_as_first() const = 0;
  virtual bool to_be_called_as_second() const = 0;
  virtual ~my_singleton_base() = default;
};

struct my_singleton_t final : my_singleton_base
{
  my_singleton_t() = default;
  my_singleton_t(my_singleton_t const&) = delete;
  my_singleton_t& operator=(my_singleton_t const&) = delete;
  my_singleton_t(my_singleton_t&&) = default;
  my_singleton_t& operator=(my_singleton_t&&) = default;

  bool to_be_called_as_first() const override { return false; }
  bool to_be_called_as_second() const override { return true; }
};

bool
perform_action(bool& first_attempt, my_singleton_base const& msbase)
{
  bool return_value{};
  if (first_attempt) {
    return_value = msbase.to_be_called_as_first();
    first_attempt = !first_attempt;
  } else {
    return_value = msbase.to_be_called_as_second();
  }
  return return_value;
}

bool
perform_action()
{
  static bool first_attempt{ true };
  my_singleton_t mst;
  return perform_action(first_attempt, mst);
}

TEST(brown_bag, e2e)
{
  ASSERT_FALSE(perform_action());
  ASSERT_TRUE(perform_action());
}

TEST(brown_bag, mocking_strategy_true)
{
  bool first_attempt{ true };
  my_singleton_t mst;
  ASSERT_FALSE(perform_action(first_attempt, mst));
}

TEST(brown_bag, mocking_strategy_false)
{
  bool first_attempt{ false };
  my_singleton_t mst;
  ASSERT_TRUE(perform_action(first_attempt, mst));
}

struct my_singleton_mock : my_singleton_base
{
  MOCK_METHOD(bool, to_be_called_as_first, (), (const));
  MOCK_METHOD(bool, to_be_called_as_second, (), (const));
};

TEST(brown_bag, mock_true)
{
  my_singleton_mock msm;
  EXPECT_CALL(msm, to_be_called_as_first).WillRepeatedly(Return(false));
  bool first_attempt{ true };
  ASSERT_FALSE(perform_action(first_attempt, msm));
}

TEST(brown_bag, mock_false)
{
  my_singleton_mock msm;
  EXPECT_CALL(msm, to_be_called_as_second).WillRepeatedly(Return(true));
  bool first_attempt{ false };
  ASSERT_TRUE(perform_action(first_attempt, msm));
}
