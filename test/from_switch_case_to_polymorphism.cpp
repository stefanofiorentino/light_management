#include <gmock/gmock.h>

#include <cstring>
#include <string>

namespace details {
char *print_foo() { return strdup(__PRETTY_FUNCTION__); }
char *print_bar() { return strdup(__PRETTY_FUNCTION__); }
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
