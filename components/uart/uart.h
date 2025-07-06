#ifndef __UART_H
#define __UART_H

#include <driver/uart.h>
#include "shearch_component.h"

#define UART_RX_PIN     16
#define UART_TX_PIN     17

#define UART_RX_BUFFER  2048
#define UART_TX_BUFFER  2048

#define UART_QUEUE_SIZE 256

void uart_init(const uint32_t baud_rate);
void vUartRxEventTask(void* pvParameters);
void vUartTxTask(void* pvParameters);


#endif /* __UART_H*/