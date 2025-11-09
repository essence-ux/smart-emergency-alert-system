#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "comms.h"

static const char *TAG = "main";

/* Define the sensor input pins */
#define PIN_FLAME GPIO_NUM_33
#define PIN_GAS   GPIO_NUM_32
#define PIN_SOS   GPIO_NUM_25

/* Convert coordinate strings from sdkconfig to doubles */
static double get_fixed_lat(void)
{
    const char *s = CONFIG_FIXED_LAT;
    return s ? atof(s) : 0.0;
}
static double get_fixed_lon(void)
{
    const char *s = CONFIG_FIXED_LON;
    return s ? atof(s) : 0.0;
}

/* Basic debounce function to avoid false triggering */
static int read_debounced(gpio_num_t pin)
{
    int stable = gpio_get_level(pin);
    for (int i = 0; i < 5; ++i) {
        vTaskDelay(pdMS_TO_TICKS(10));
        int v = gpio_get_level(pin);
        if (v != stable) return -1; // unstable reading
    }
    return stable;
}

/* Task that monitors sensors continuously */
void sensor_task(void *arg)
{
    double lat = get_fixed_lat();
    double lon = get_fixed_lon();

    while (1) {
        int flame = read_debounced(PIN_FLAME);
        int gas   = read_debounced(PIN_GAS);
        int sos   = read_debounced(PIN_SOS);

        // Adjust active levels depending on your hardware logic
        if (flame == 0) {
            ESP_LOGI(TAG, "Flame detected — sending alert");
            comms_send_alert("flame", lat, lon);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        if (gas == 1) {
            ESP_LOGI(TAG, "Gas leak detected — sending alert");
            comms_send_alert("gas", lat, lon);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        if (sos == 1) {
            ESP_LOGI(TAG, "SOS button pressed — sending alert");
            comms_send_alert("sos", lat, lon);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

/* Main application entry */
void app_main(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Starting Smart Emergency Alert System (ESP32 + Blynk)");

    /* Initialize NVS (non-volatile storage) required by Wi-Fi */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Configure sensor pins as input */
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << PIN_FLAME) | (1ULL << PIN_GAS) | (1ULL << PIN_SOS),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    /* Initialize Wi-Fi + Blynk communication */
    int r = comms_init();
    if (r != 0) {
        ESP_LOGW(TAG, "comms_init failed (%d). Continuing; alerts will fail until Wi-Fi connects.", r);
    }

    /* Create the sensor monitoring task */
    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}
