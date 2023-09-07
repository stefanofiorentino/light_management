#include <gmock/gmock.h>

// prod.h
struct context_interface
{
  virtual void* zmq_ctx_new() = 0;
};

bool
monitor_PUB_ZMQ(context_interface* context);

// prod.cpp
bool
monitor_PUB_ZMQ(context_interface* context)
{
  void* ctx = context->zmq_ctx_new();
  if (!ctx) {
    return false;
  }
  return true;
}

// test.cpp
struct context_mock : context_interface
{
  MOCK_METHOD(void*, zmq_ctx_new, (), (override));
};

TEST(mob, givenValidContext_whenCallMonitorPubZMQ_thenReturnTrue)
{
  context_mock c;
  EXPECT_CALL(c, zmq_ctx_new).WillOnce([] {
    static int i;
    return &i;
  });
  ASSERT_TRUE(monitor_PUB_ZMQ(&c));
}

TEST(mob, givenInvalidContext_whenCallMonitorPubZMQ_thenReturnFalse)
{
  context_mock c;
  EXPECT_CALL(c, zmq_ctx_new).WillOnce([] { return nullptr; });
  ASSERT_FALSE(monitor_PUB_ZMQ(&c));
}
