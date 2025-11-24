/* #include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.h"
#include "mqtt_pub_sub.h"

void app_main(void)
{

    // TO MAKE FREE UP ANY ALLOCATED HEAP MEMORY AND TO PRINT THE EDF VERSION
    ESP_LOGI("MAIN", "[APP] Startup..");
    ESP_LOGI("MAIN", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());
    //

    nvs_flash_init();

    // Connect WiFi
    connect_wifi();

    // Start MQTT (connect + subscribe + auto publish logic preserved)
    mqtt_pubsub_start();

    // Publish every 5 seconds
    while (1)
    {
        mqtt_publish("ESP32/values", "Periodic message", 1);
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 sek
    }
} */
/*
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "wifi.h"
#include "mqtt_broker.h"

void app_main(void)
{
    // TO MAKE FREE UP ANY ALLOCATED HEAP MEMORY AND TO PRINT THE EDF VERSION
    ESP_LOGI("MAIN", "[APP] Startup..");
    ESP_LOGI("MAIN", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());
    //

    // Always initialize the default NVS partition first
    nvs_flash_init();

    // start WIFI connection
    connect_wifi();

    // Setup AND Start mqtt
    // mqtt_pubsub_start();

    // OR

    //...
    // Set ESP as Broker
    mqtt_broker_start();
} */

#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "wifi.h"
#include "esp_https_server.h"
#include "http_server.h"
#include "mqtt_pub_sub.h"
#include "mqtt_broker.h"

void app_main(void)
{
    // TO MAKE FREE UP ANY ALLOCATED HEAP MEMORY AND TO PRINT THE EDF VERSION
    ESP_LOGI("MAIN", "[APP] Startup..");
    ESP_LOGI("MAIN", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());
    //

    // Always initialize the default NVS partition first
    nvs_flash_init();

    // start WIFI connection
    connect_wifi();

    // Start MQTT BROKER --> CORE 0
    xTaskCreatePinnedToCore(mqtt_broker_start, "MQTT BROKER TASK - CORE 0", 4096, NULL, 1, NULL, 0);

    // Start HTTP Server on --> CORE 1
    xTaskCreatePinnedToCore(http_server_start, "HTTP SERVER TASK - CORE 1", 4096, NULL, 1, NULL, 1);
}