#include <gmock/gmock.h>
#include <sstream>

template<typename T>
class wrap
{
  const std::string P_START_TAG{ "<p>" };
  const std::string P_END_TAG{ "</p>" };

  void paragraph_start(std::ostringstream& oss) const { oss << P_START_TAG; }

  void paragraph_end(std::ostringstream& oss) const { oss << P_END_TAG; }

  virtual void do_paragraph(std::ostringstream& oss,
                            std::istringstream const& iss) const = 0;

public:
  void paragraph(std::ostringstream& oss, std::istringstream const& iss) const
  {
    paragraph_start(oss);
    static_cast<const T*>(this)->do_paragraph(oss, iss);
    paragraph_end(oss);
  }
};

class foo final : public wrap<foo>
{
  friend class wrap<foo>;
  void do_paragraph(std::ostringstream& oss,
                    std::istringstream const& iss) const override
  {
    oss << iss.str();
  }
};

TEST(crtp, nvi)
{
  // Arrange
  foo f;
  std::istringstream iss("text");
  std::ostringstream oss;

  // Act
  f.paragraph(oss, iss);

  // Assert
  ASSERT_EQ("<p>text</p>", oss.str());
}
