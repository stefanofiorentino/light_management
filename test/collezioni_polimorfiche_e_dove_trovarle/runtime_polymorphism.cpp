#include <gmock/gmock.h>

#include <document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg

TEST(runtime_polymorphism, draw_plain_plant) {
  collection_t c;
  c.emplace_back(42);
  auto s = std::string("a_string");
  c.emplace_back(s);

  collection_t c1;
  c1.emplace_back(24);
  c.emplace_back(std::move(c1));

  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ(R"(<collection>
  42
  a_string
  <collection>
    24
  </collection>
</collection>
)",
            oss.str());
}