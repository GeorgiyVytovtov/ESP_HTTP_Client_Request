#include <unity.h>
#include "test_http.h"
#include "http.h"

void wifi_init_test(void)
{
    wifi_init();
    TEST_ASSERT_TRUE(true);
}

void run_http_tests(void)
{
    UNITY_BEGIN();
    RUN_TEST(wifi_init_test);
    UNITY_END();
}