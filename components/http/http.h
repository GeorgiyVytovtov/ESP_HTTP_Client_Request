#ifndef __HTTP_H
#define __HTTP_H
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "shearch_component.h"
#include "password.h"

typedef struct{
    char typeHttpRequest[128];
    char url[128];
    char body[128];
}HttpRequest;

void wifi_init(void);
void vHttpRequestTask(void* pvParameters);
void http_get(const char* url);
void http_post(const char* url, const char* body);

#endif /* __HTTP_H */