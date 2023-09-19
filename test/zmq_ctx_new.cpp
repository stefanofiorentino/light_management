#include <gmock/gmock.h>

// prod.h
struct context_interface
{
  virtual void* ctx_new() = 0;
};

bool
thread_func(context_interface* context);

// prod.cpp
bool
thread_func(context_interface* context)
{
  void* ctx = context->ctx_new();
  if (!ctx) {
    return false;
  }
  return true;
}

// test.cpp
struct context_mock : context_interface
{
  MOCK_METHOD(void*, ctx_new, (), (override));
};

TEST(mob, givenValidContext_whenCallMonitorPubthenReturnTrue)
{
  context_mock c;
  EXPECT_CALL(c, ctx_new).WillOnce([] {
    static int i;
    return &i;
  });
  ASSERT_TRUE(thread_func(&c));
}

TEST(mob, givenInvalidContext_whenCallMonitorPubthenReturnFalse)
{
  context_mock c;
  EXPECT_CALL(c, ctx_new).WillOnce([] { return nullptr; });
  ASSERT_FALSE(thread_func(&c));
}
