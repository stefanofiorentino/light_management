#include <check.h>

START_TEST(test)
{
    ck_assert_str_eq("1", "1");
}
END_TEST

Suite* create(void)
{
    Suite* suite;
    TCase* core;

    suite = suite_create("test");
    core = tcase_create("core");
    tcase_add_test(core, test);
    suite_add_tcase(suite, core);
    
    return suite;
}

int main(void)
{
    int failed;
    Suite* suite;
    SRunner *runner;

    suite = create();
    runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed==0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
