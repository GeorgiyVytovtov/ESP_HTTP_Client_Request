#include <stdio.h>
#include "uart.h"
#include "parse.h"
#include "http.h"

void app_main(void)
{
    nvs_flash_init();
    uart_init(115200);
    wifi_init();
    xTaskCreate(vUartRxEventTask, "vUartRxEventTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vParseTask, "vParseTask", 1024*4, NULL, 5, NULL);
    while (1)
    {
        vTaskDelay( 500 / portTICK_PERIOD_MS);
    }
    
}