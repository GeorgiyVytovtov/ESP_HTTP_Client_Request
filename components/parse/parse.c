#include "parse.h"

HttpRequest parse_message()
{
    HttpRequest httpRequest = {0};
    return httpRequest;
}

void vParseTask(void* pvParameters)
{
    CommunicationData data;
    http_request_queue = xQueueCreate(10, sizeof(HttpRequest));
    while(1)
    {
        if(xQueueReceive(uart_rx_msg_queue, &data, portMAX_DELAY))
        {
            HttpRequest http_request = parse_message(data.buffer, data.len);
            xQueueSend(http_request_queue, &http_request, NULL);
        }
    }
}