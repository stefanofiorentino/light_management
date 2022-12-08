#include <gmock/gmock.h>

template<typename T>
class wrap
{
  const std::string P_START_TAG{ "<p>" };
  const std::string P_END_TAG{ "</p>" };

  std::string paragraph_start() const { return P_START_TAG; }

  std::string paragraph_end() const { return P_END_TAG; }

public:
  std::string paragraph(std::string const& text) const
  {
    return paragraph_start() +
           static_cast<const T*>(this)->paragraph_customization(text) +
           paragraph_end();
  }
};

class foo final : public wrap<foo>
{
  friend class wrap<foo>;
  std::string paragraph_customization(std::string const& text) const
  {
    return text;
  }
};

TEST(crtp, nvi)
{
  foo f;
  ASSERT_EQ("<p>text</p>", f.paragraph("text"));
}
