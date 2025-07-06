#include <unity.h>
#include "test_parse.h"
#include "parse.h"

void test_parse_valid_http_message()
{
    const char *message = "http GET example.com/path {\"temp\":42}";
    ParseHttpRequest result = parse_message(message, strlen(message));

    TEST_ASSERT_TRUE(result.is_correct_parse);
    TEST_ASSERT_EQUAL_STRING("GET", result.httpRequest.typeHttpRequest);
    TEST_ASSERT_EQUAL_STRING("example.com/path", result.httpRequest.url);
    TEST_ASSERT_EQUAL_STRING("{\"temp\":42}", result.httpRequest.body);
}

void test_parse_fail_on_invalid_message()
{
    const char *message = "invalid_format";
    ParseHttpRequest result = parse_message(message, strlen(message));

    TEST_ASSERT_FALSE(result.is_correct_parse);
}

void test_parse_valid_no_body()
{
    const char *input = "http GET http://httpbin.org";
    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_TRUE(result.is_correct_parse);
    TEST_ASSERT_EQUAL_STRING("GET", result.httpRequest.typeHttpRequest);
    TEST_ASSERT_EQUAL_STRING("http://httpbin.org", result.httpRequest.url);
    TEST_ASSERT_EQUAL_STRING("", result.httpRequest.body);
}

void test_parse_valid_no_body_with_space()
{
    const char *input = "http GET http://httpbin.org ";
    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_TRUE(result.is_correct_parse);
    TEST_ASSERT_EQUAL_STRING("GET", result.httpRequest.typeHttpRequest);
    TEST_ASSERT_EQUAL_STRING("http://httpbin.org", result.httpRequest.url);
    TEST_ASSERT_EQUAL_STRING("", result.httpRequest.body);
}

void test_parse_fail_on_no_space_after_type()
{
    const char *input = "httpGET http://httpbin.org";
    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_FALSE(result.is_correct_parse);
    TEST_ASSERT_NOT_NULL(strstr(result.error_msg, "correct input data"));
}

void test_parse_fail_on_type_too_long()
{
    char long_type[50] = {0};
    memset(long_type, 'A', sizeof(long_type) - 1);

    char input[100] = {0};
    snprintf(input, sizeof(input), "http %s /url", long_type);

    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_FALSE(result.is_correct_parse);
    TEST_ASSERT_NOT_NULL(strstr(result.error_msg, "request type too long"));
}

void test_parse_fail_on_url_too_long()
{
    char long_url[200] = {0};
    memset(long_url, 'U', sizeof(long_url) - 1);

    char input[300] = {0};
    snprintf(input, sizeof(input), "http GET %s body", long_url);

    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_FALSE(result.is_correct_parse);
    TEST_ASSERT_NOT_NULL(strstr(result.error_msg, "so long url"));
}

void test_parse_fail_on_body_too_long()
{
    char long_body[200] = {0};
    memset(long_body, 'B', sizeof(long_body) - 1);

    char input[300] = {0};
    snprintf(input, sizeof(input), "http GET /url %s", long_body);

    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_FALSE(result.is_correct_parse);
    TEST_ASSERT_NOT_NULL(strstr(result.error_msg, "so long body"));
}

void test_parse_fail_on_non_http_prefix()
{
    const char *input = "ftp GET /url body";
    ParseHttpRequest result = parse_message(input, strlen(input));

    TEST_ASSERT_FALSE(result.is_correct_parse);
    TEST_ASSERT_NOT_NULL(strstr(result.error_msg, "correct input data"));
}

void run_parse_tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_parse_valid_http_message);
    RUN_TEST(test_parse_fail_on_invalid_message);
    RUN_TEST(test_parse_valid_no_body);
    RUN_TEST(test_parse_valid_no_body_with_space);
    RUN_TEST(test_parse_fail_on_no_space_after_type);
    RUN_TEST(test_parse_fail_on_type_too_long);
    RUN_TEST(test_parse_fail_on_url_too_long);
    RUN_TEST(test_parse_fail_on_body_too_long);
    RUN_TEST(test_parse_fail_on_non_http_prefix);
    UNITY_END();
}
