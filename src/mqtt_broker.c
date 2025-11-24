#include "mqtt_broker.h"
#include "mosq_broker.h"
#include "esp_log.h"

static const char *TAG = "MQTT_BROKER";

// Define the variable here
char latest_temp[32] = "0";

void mqtt_message_cb(char *client, char *topic, char *data, int len, int qos, int retain)
{
    if (strcmp(topic, "ESP32/values") == 0)
    {
        // Copy the message into our global variable
        int copy_len = len < sizeof(latest_temp) - 1 ? len : sizeof(latest_temp) - 1;
        memcpy(latest_temp, data, copy_len);
        latest_temp[copy_len] = '\0';
    }
}

void mqtt_broker_start(void *pvParameters)
{
    struct mosq_broker_config config = {
        .host = "0.0.0.0",
        .port = 1883,
        .tls_cfg = NULL,
        .handle_message_cb = mqtt_message_cb // <-- set callback
    };

    ESP_LOGI(TAG, "Starting MQTT Broker on port %d", config.port);

    mosq_broker_run(&config); // blocking
    mosq_broker_stop();
}
