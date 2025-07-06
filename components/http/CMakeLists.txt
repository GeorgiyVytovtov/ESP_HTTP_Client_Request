idf_component_register(SRCS "http.c"
                    REQUIRES shearch_component
                    REQUIRES 
                        esp_wifi
                        esp_event
                        nvs_flash
                        esp_netif
                        esp_http_client
                    INCLUDE_DIRS ".")
