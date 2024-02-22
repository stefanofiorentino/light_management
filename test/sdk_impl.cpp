#include <gmock/gmock.h>
#include <sdk/sdk.h>

struct context_t
{
  bool called{ false };
};

void
onCommissioningInfo(const char*, const char*, void* context)
{
  auto c = static_cast<context_t*>(context);
  c->called = true;
}

TEST(sdk, onCommisioning_event_fired)
{
  context_t c;
  ASSERT_FALSE(c.called);
  commissioningInit(&c);

  auto message = R"({
     "email": "email@email.com"
    ,"password": "123456"
})";
  startCommissioning(message);
  ASSERT_TRUE(c.called);
}
