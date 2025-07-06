#include "uart.h"

static const char* TAG = "UART";

const uart_port_t  uart_num = UART_NUM_1;

QueueHandle_t uart_rx_queue = NULL;
QueueHandle_t uart_rx_msg_queue = NULL;
QueueHandle_t uart_tx_msg_queue = NULL;

static uint8_t uart_rx_buffer[UART_RX_BUFFER];
static uint8_t uart_tx_buffer[UART_TX_BUFFER];

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

void vUartRxEventTask(void* pvParameters){
    uart_event_t rx_data_event;
    uart_rx_msg_queue = xQueueCreate(10, sizeof(CommunicationData));
    while(1)
    {
        if(xQueueReceive(uart_rx_queue, (void*)&rx_data_event, portMAX_DELAY))
        {
            if(rx_data_event.type == UART_DATA)
            {
                CommunicationData data;
                uint32_t len = uart_read_bytes(uart_num, uart_rx_buffer, rx_data_event.size, portMAX_DELAY);
                if (len > UART_RX_BUFFER) {
                    len = UART_RX_BUFFER;  
                }
                data.len = len;
                
                memcpy(data.buffer, uart_rx_buffer, len);
                
                if (len < UART_RX_BUFFER) {
                    data.buffer[len] = '\0'; 
                }

                xQueueSend(uart_rx_msg_queue, &data, NULL);
            }
        }
    }
}