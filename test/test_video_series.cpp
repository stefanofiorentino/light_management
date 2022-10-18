#include <gmock/gmock.h>

struct base {
    bool empty() const {
        return true;
    }
};

TEST(test_video_series, simple)
{
    base b;
    ASSERT_TRUE(b.empty());
}