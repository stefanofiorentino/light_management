#include <gmock/gmock.h>

#include <cstring>
#include <memory>
#include <string>

namespace details {
char *print_foo() { return strdup("char* details::print_foo()"); }
char *print_bar() { return strdup("char* details::print_bar()"); }
} // namespace details

char *perform_action_based_on_type(std::string const &action_to_perform) {
  if ("foo" == action_to_perform) {
    return details::print_foo();
  } else if ("bar" == action_to_perform) {
    return details::print_bar();
  }
}

TEST(from_switch_case_to_polymorphism, C_style_polymorphism) {
  ASSERT_EQ(0, strcmp("char* details::print_foo()",
                      perform_action_based_on_type("foo")));
  ASSERT_EQ(0, strcmp("char* details::print_bar()",
                      perform_action_based_on_type("bar")));
}

class base {
protected:
  virtual char *do_print() const = 0;

public:
  char *print() const { return do_print(); }
};
class foo : public base {
  char *do_print() const override { return details::print_foo(); }
};

class bar : public base {
  char *do_print() const override { return details::print_bar(); }
};

std::shared_ptr<base> factory(std::string const &action_to_perform) {
  if ("foo" == action_to_perform) {
    return std::make_shared<foo>();
  } else if ("bar" == action_to_perform) {
    return std::make_shared<bar>();
  }
}

TEST(from_switch_case_to_polymorphism, Cpp_style_polymorphism) {
  ASSERT_EQ(0, strcmp("char* details::print_foo()",
                      factory("foo")->print()));
  ASSERT_EQ(0, strcmp("char* details::print_bar()",
                      factory("bar")->print()));
}
