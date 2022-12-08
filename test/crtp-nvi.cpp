#include <gmock/gmock.h>

template<typename T>
class wrap
{

  std::string paragraph_start() const { return "<p>"; }

  std::string paragraph_end() const { return "</p>"; }

public:
  std::string paragraph(std::string const& text) const
  {
    return paragraph_start() +
           static_cast<const T*>(this)->paragraph_customization(text) + paragraph_end();
  }
};

class foo final : public wrap<foo>
{
  friend class wrap<foo>;
  std::string paragraph_customization(std::string const& text) const { return text; }
};

TEST(crtp, nvi)
{
  foo f;
  ASSERT_EQ("<p>text</p>", f.paragraph("text"));
}
