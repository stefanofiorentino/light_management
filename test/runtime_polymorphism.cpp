#include <gmock/gmock.h>

#include <document.hpp>

struct my_class_t {
    bool reset() const {
        return false;
    }
};

void draw(const my_class_t&, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "my_class_t" << "\n";
}

void test_helper(std::ostream& out)
{    
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

struct light_management_fixture : ::testing::Test
{
    std::ostringstream oss;
};

TEST_F(light_management_fixture, runtime_polymorphism)
{
    test_helper(oss);
    ASSERT_EQ(R"(<document>
  my_class_t
</document>
)", oss.str());
}
