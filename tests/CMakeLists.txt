idf_component_register(
    SRCS "test_http.c" "test_uart.c"
    INCLUDE_DIRS "."
    REQUIRES uart unity  http parse
)