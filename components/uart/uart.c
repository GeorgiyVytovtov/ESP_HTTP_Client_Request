#include "uart.h"

static const char* TAG = "UART";

const uart_port_t  uart_num = UART_NUM_1;

QueueHandle_t uart_rx_queue = NULL;

void uart_init(const uint32_t baud_rate)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(uart_num, UART_RX_BUFFER, UART_RX_BUFFER, UART_QUEUE_SIZE, &uart_rx_queue, NULL));
}