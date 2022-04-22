#include <gmock/gmock.h>

#include <document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg

struct my_struct_t {
  char const *const name = "my_struct_t";
};
template <>
void draw(my_struct_t const &x, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << x.name << "\n";
}

TEST(runtime_polymorphism, draw_plain_plant) {
  collection_t c;
  c.emplace_back(42);
  auto s = std::string("a_string");
  c.emplace_back(s);

  c.emplace_back(c);
  c.emplace_back(my_struct_t());
  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ(R"(<collection>
  42
  a_string
  <collection>
    42
    a_string
  </collection>
  my_struct_t
</collection>
)",
            oss.str());
}