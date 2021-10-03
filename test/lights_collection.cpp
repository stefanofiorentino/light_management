#include <gmock/gmock.h>

#include <iostream>

#include <document.hpp>

struct my_class_t {
    void draw(std::ostream& out) const {
        out << "my_class_t";
    }
    void switchOff(std::ostream& out)
    {
        out << "switching OFF";
    }
};

void draw(const my_class_t& my_class, std::ostream& out, size_t position)
{
    out << std::string(position, ' ');
    my_class.draw(out);
    out << "\n";
}

void switchOff(my_class_t& my_class, std::ostream& out, size_t position)
{
    out << std::string(position, ' ');
    my_class.switchOff(out);
    out << "\n";
}

struct light_management_fixture : ::testing::Test
{
    std::ostringstream oss;
};

TEST_F(light_management_fixture, runtime_polymorphism)
{
    document_t room;
    room.emplace_back(my_class_t());

    switchOff(room, oss, 0);
    ASSERT_EQ("switching ALL OFF\n", oss.str());
}
