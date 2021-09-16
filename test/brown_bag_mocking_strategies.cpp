#include <gmock/gmock.h>

#include <iostream>

struct my_singleton_t 
{
    static my_singleton_t* instance() 
    {
        static my_singleton_t my_singleton;
        return &my_singleton;
    }

    bool to_be_called_as_first() const {
        std::puts(__FUNCSIG__);
        return false;
    }
    bool to_be_called_as_second() const {
        std::puts(__FUNCSIG__);
        return true;
    }
};

bool perform_action()
{
    static bool first_attempt{true};
    bool return_value{};
    if (first_attempt)
    {
        return_value = my_singleton_t::instance()->to_be_called_as_first();
        first_attempt = !first_attempt;
    }
    else
    {
        return_value = my_singleton_t::instance()->to_be_called_as_second();
    }
    return return_value;
}

TEST(brown_bag, mocking_strategy)
{
    ASSERT_FALSE(perform_action());
    ASSERT_TRUE(perform_action());
}
