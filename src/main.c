
// ---- OLED DISPLAY -----
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ssd1306.h>

#define I2C_SDA 21 // SERIAL DATA LINE
#define I2C_SCL 22 // SERIAL CLOCK LINE

void app_main(void)
{
    // Display initialisieren (Standard-Adresse 0x3C)
    ssd1306_128x64_i2c_initEx(I2C_SCL, I2C_SDA, 0);
    ssd1306_clearScreen();
    ssd1306_setFixedFont(ssd1306xled_font8x16);

    // Bildschirm löschen
    ssd1306_clearScreen();

    // Text ausgeben
    ssd1306_printFixed(0, 10, "Hello World!", STYLE_NORMAL);
}

/*

 ---- ULTRASONIC ----

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ultrasonic.h"
#include <inttypes.h>

#define ULTRASONIC_TASK_STACK_SIZE 2048  // Stack in Bytes (pro Task)
#define ULTRASONIC_TASK_PRIORITY   5     // Priorität (0–configMAX_PRIORITIES-1)

static void ultrasonic_task(void *pvParameters)
{
    // Zeiger auf das übergebene Sensor-Objekt
    ultrasonic_sensor_t *sensor = (ultrasonic_sensor_t *)pvParameters;

    while (1)
    {
        uint32_t distance_cm;
        esp_err_t res = ultrasonic_measure_cm(sensor, &distance_cm);

        if (res == ESP_OK)
            printf("Entfernung: %" PRIu32 " cm\n", distance_cm);
        else
            // printf("Messfehler: %s\n", esp_err_to_name(res));

        vTaskDelay(pdMS_TO_TICKS(500)); // 500 ms Pause
    }
}

void app_main(void)
{
    static ultrasonic_sensor_t sensor = {
        .sig_pin = GPIO_NUM_19 // SIG-Pin
    };

    ultrasonic_init(&sensor);

    // Task starten
    xTaskCreate(
        ultrasonic_task,              // Task-Funktion
        "ultrasonic_task",            // Name
        ULTRASONIC_TASK_STACK_SIZE,   // Stackgröße in Bytes
        &sensor,                      // Parameter (Pointer auf Sensor)
        ULTRASONIC_TASK_PRIORITY,     // Task-Priorität
        NULL                          // Option: Handle (nicht gebraucht)
    );
}



*/