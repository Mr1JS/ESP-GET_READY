#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <iot_servo.h>
#include <esp_err.h>

/* ----- Configuration ----- */
#define SERVO_GPIO 18           /**< GPIO connected to the servo */
#define SERVO_CALIB_0_DEG 30    /**< Servo calibration value for 0° */
#define SERVO_CALIB_180_DEG 195 /**< Servo calibration value for 180° */

esp_err_t servo_init(void);

void servo_start_task(void *pvParameters);
