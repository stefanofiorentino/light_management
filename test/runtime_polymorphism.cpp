#include <gmock/gmock.h>

#include <document.hpp>

struct my_class_t {
  void draw(std::ostream &out) const { out << "my_class_t"; }
};

template <>
void draw(const my_class_t &my_class, std::ostream &out, size_t position) {
  out << std::string(position, ' ');
  my_class.draw(out);
  out << "\n";
}

void test_helper(std::ostream &out) {
  document_t document;
  document.reserve(5);

  document.emplace_back(0);
  document.emplace_back(std::string("Hello!"));
  document.emplace_back(document);
  document.emplace_back(my_class_t());
  document.emplace_back(4);

  std::reverse(std::begin(document), std::end(document));
  draw(document, out, 0);
}

struct light_management_fixture : ::testing::Test {
  std::ostringstream oss;
};

TEST_F(light_management_fixture, runtime_polymorphism) {
  test_helper(oss);
  ASSERT_EQ("<document>\n  4\n  my_class_t\n  <document>\n    0\n    Hello!\n  "
            "</document>\n  Hello!\n  0\n</document>\n",
            oss.str());
}
