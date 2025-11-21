#include "servo_sensor.h"
#include "esp_log.h"

static const char *TAG = "Servo_Task";

esp_err_t servo_init(void)
{
    servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 500,
        .max_width_us = 2500,
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {SERVO_GPIO},
            .ch = {LEDC_CHANNEL_0},
        },
        .channel_number = 1,
    };

    esp_err_t res = iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize servo: %s", esp_err_to_name(res));
        return res;
    }

    ESP_LOGI(TAG, "Servo initialized successfully on GPIO %d", SERVO_GPIO);
    return ESP_OK;
}

// The task function
void servo_start_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting servo task");

    while (1)
    {
        // Sweep from 0° to 180°
        for (int i = SERVO_CALIB_0_DEG; i <= SERVO_CALIB_180_DEG; i++)
        {
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, i);
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        // Return to 0°
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, SERVO_CALIB_0_DEG);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
