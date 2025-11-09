idf_component_register(SRCS "comms.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_http_client esp_wifi nvs_flash)
