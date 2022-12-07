#include <gmock/gmock.h>

template<typename T>
class wrap
{
  std::string to_string_pre() const { return "pre-"; }

  std::string to_string_post() const { return "-post"; }

public:
  std::string to_string() const
  {
    return to_string_pre() + static_cast<const T*>(this)->to_string_impl() +
           to_string_post();
  }
};

class foo final : public wrap<foo>
{
public:
  std::string to_string_impl() const { return "inside"; }
};

TEST(crtp, nvi)
{
  ASSERT_EQ("pre-inside-post", wrap<foo>().to_string());
}