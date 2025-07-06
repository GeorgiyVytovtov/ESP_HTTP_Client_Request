#include "parse.h"

static const char *TAG = "PARSE";

typedef struct
{
    HttpRequest httpRequest;
    bool is_correct_parse;
} ParseHttpRequest;

ParseHttpRequest parse_message(const char *data, uint16_t size)
{
    ParseHttpRequest httpRequest = {0};

    if (size > 5 && strncmp(data, "http", 4) == 0)
    {
        const char *ptr_start_request_type = data + 5;
        const char *ptr_end_request_type = strchr(ptr_start_request_type, ' ');
        if (!ptr_end_request_type)
        {
            ESP_LOGI(TAG, "Error: haven't completed correct input data");
            return httpRequest;
        }

        uint32_t type_request_len = ptr_end_request_type - ptr_start_request_type;
        if (type_request_len >= sizeof(httpRequest.httpRequest.typeHttpRequest))
        {
            ESP_LOGI(TAG, "Error: request type too long");
            return httpRequest;
        }
        strncpy(httpRequest.httpRequest.typeHttpRequest, ptr_start_request_type, type_request_len);
        httpRequest.httpRequest.typeHttpRequest[type_request_len] = '\0';

        const char *ptr_start_url = ptr_end_request_type + 1;
        const char *ptr_end_url = strchr(ptr_start_url, ' ');
        /*
        if (!ptr_end_url){
            size_t url_len = (data+size) - ptr_start_url;
            strncpy(httpRequest.httpRequest.url, ptr_start_url, url_len);
            httpRequest.httpRequest.url[url_len] = '\0';
            httpRequest.is_correct_parse = true;
            return httpRequest;
        }*/

        size_t url_len = ptr_end_url - ptr_start_url; //!
        if (url_len >= sizeof(httpRequest.httpRequest.url))
        {
            ESP_LOGI(TAG, "Error: so long url");
            httpRequest.is_correct_parse = false;
            return httpRequest;
        }
        strncpy(httpRequest.httpRequest.url, ptr_start_url, url_len);
        httpRequest.httpRequest.url[url_len] = '\0';

        const char *ptr_start_body = ptr_end_url + 1;
        size_t body_len = size - (ptr_start_body - data);
        if (body_len >= sizeof(httpRequest.httpRequest.body))
        {
            ESP_LOGI(TAG, "Error: so long body");
            httpRequest.is_correct_parse = false;
            return httpRequest;
        }
        strncpy(httpRequest.httpRequest.body, ptr_start_body, body_len);
        httpRequest.httpRequest.body[body_len] = '\0';
    }
    httpRequest.is_correct_parse = true;
    return httpRequest;
}

void vParseTask(void *pvParameters)
{
    CommunicationData data;
    http_request_queue = xQueueCreate(10, sizeof(HttpRequest));
    while (1)
    {
        if (xQueueReceive(uart_rx_msg_queue, &data, portMAX_DELAY))
        {
            ParseHttpRequest parse_http_request = parse_message(data.buffer, data.len);
            ESP_LOGI(TAG,"pars");
            if (parse_http_request.is_correct_parse)
            {
                // xQueueSend(http_request_queue, &parse_http_request.httpRequest, NULL);
                ESP_LOGI(TAG, "url: %s", parse_http_request.httpRequest.url);
                ESP_LOGI(TAG, "typeHttpRequest: %s", parse_http_request.httpRequest.typeHttpRequest);
                ESP_LOGI(TAG, "body: %s", parse_http_request.httpRequest.body);
            }
        }
    }
}