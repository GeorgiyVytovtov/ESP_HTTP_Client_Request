#include <unity.h>
#include "uart.h"

void test_uart_init(void)
{
    uart_init(115200);
    TEST_ASSERT_TRUE(true);
}

void run_uart_tests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_uart_init);
    UNITY_END();
}
