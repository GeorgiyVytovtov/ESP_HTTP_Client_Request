idf_component_register(
    SRCS "test_parse.c" "test_http.c" "test_uart.c"
    INCLUDE_DIRS "."
    REQUIRES unity uart http parse
)