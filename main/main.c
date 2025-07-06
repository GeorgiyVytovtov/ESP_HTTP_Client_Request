#include <stdio.h>
#include "uart.h"
#include "parse.h"
#include "http.h"
#include "test_uart.h"
#include "test_http.h"

/*
void app_main(void)
{
    nvs_flash_init();
    uart_init(115200);
    wifi_init();
    xTaskCreate(vUartRxEventTask, "vUartRxEventTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vParseTask, "vParseTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vHttpRequestTask, "vHttpRequestTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vUartTxTask, "vUartTxTask", 1024*3, NULL, 5, NULL);
}*/

void app_main(void)
{
    nvs_flash_init();
    run_uart_tests();
    run_http_tests();
}