#include "http.h"
#include "parse.h"

QueueHandle_t http_request_queue = NULL;

static SemaphoreHandle_t http_request_semaphore;

#define WIFI_CONNECTED_BIT BIT0
static EventGroupHandle_t wifi_event_group;
static const char *TAG = "HTTP";

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGW(TAG, "WiFi lost connection, reconnecting...");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "Got IP!");
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t client_event_handler(esp_http_client_event_handle_t event)
{
    switch (event->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        if (!esp_http_client_is_chunked_response(event->client))
        {
            CommunicationData output;
            uint32_t copy_len = event->data_len;
            if (copy_len > sizeof(output.buffer))
            {
                ESP_LOGI(TAG, "Overflow %d", event->data_len);
                copy_len = sizeof(output.buffer) - 1;
            }

            memcpy(output.buffer, event->data, copy_len);
            output.len = copy_len;

            if (xQueueSend(uart_tx_msg_queue, &output, pdMS_TO_TICKS(100)) != pdPASS)
            {
                ESP_LOGW(TAG, "Queue full — data chunk lost");
            }
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "End message data");
        xSemaphoreGive(http_request_semaphore);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init()
{
    wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "WiFi were init, wait for connection...");

    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
}

void http_get(const char *url)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .event_handler = client_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP GET Status = %d", esp_http_client_get_status_code(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET Request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

void http_post(const char *url, const char *body)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .event_handler = client_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "User-Agent", "ESP32 HTTP Client/1.0");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP POST Status = %d", esp_http_client_get_status_code(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP POST Request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

void vHttpRequestTask(void *pvParameters)
{
    HttpRequest httpRequest;
    http_request_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(http_request_semaphore);
    while (1)
    {
        if (xQueueReceive(http_request_queue, &httpRequest, portMAX_DELAY))
        {
            if (xSemaphoreTake(http_request_semaphore, 0) == pdTRUE)
            {
                ESP_LOGI(TAG, "url: %s", httpRequest.url);
                ESP_LOGI(TAG, "typeHttpRequest: %s", httpRequest.typeHttpRequest);
                ESP_LOGI(TAG, "body: %s", httpRequest.body);
                if (strcmp("GET", httpRequest.typeHttpRequest) == 0)
                {
                    http_get(httpRequest.url);
                }
                else if (strcmp("POST", httpRequest.typeHttpRequest) == 0)
                {
                    http_post(httpRequest.url, httpRequest.body);
                }
            }
        }
    }
}
