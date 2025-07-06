#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern QueueHandle_t uart_rx_msg_queue;
extern QueueHandle_t uart_tx_msg_queue;
extern QueueHandle_t http_request_queue;