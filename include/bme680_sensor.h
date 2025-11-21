#pragma once
#include "esp_err.h"
#include "bme680.h"

// ============================ CONSTANTS ============================

/**
 * @brief Default I2C address of BME680 sensor
 */
#define BME680_I2C_ADDRESS 0x76

/**
 * @brief Alternative I2C address of BME680 sensor
 */
#define BME680_I2C_ADDRESS_ALT 0x77

/**
 * @brief Default I2C speed in Hz
 */
#define BME680_I2C_SPEED 400000

/**
 * @brief Default measurement interval in milliseconds
 */
#define BME680_MEASUREMENT_INTERVAL_MS 5000

/**
 * @brief Maximum number of retry attempts for sensor initialization
 */
#define BME680_MAX_INIT_RETRIES 3

/**
 * @brief Time in ms to wait between initialization retry attempts
 */
#define BME680_INIT_RETRY_DELAY_MS 100

// ============================ FUNCTIONS ============================

/**
 * @brief Initializes the BME680 sensor
 *
 * @return esp_err_t ESP_OK on success, otherwise error code
 */
esp_err_t bme680_sensor_init(void);

/**
 * @brief Starts sensor measurement
 *
 * @return esp_err_t ESP_OK on success, otherwise error code
 */
esp_err_t bme680_sensor_start_measurement(void);

/**
 * @brief Reads sensor data
 *
 * @param temperature Pointer for temperature output (°C)
 * @param humidity Pointer for humidity output (%)
 * @param pressure Pointer for pressure output (hPa)
 * @param gas_resistance Pointer for gas resistance output (kΩ)
 * @return esp_err_t ESP_OK on success, otherwise error code
 */
esp_err_t bme680_sensor_read_data(float *temperature, float *humidity,
                                  float *pressure, float *gas_resistance);

/**
 * @brief Returns the sensor handle (for advanced functions)
 *
 * @return bme680_handle_t Sensor handle or NULL
 */
bme680_handle_t bme680_sensor_get_handle(void);

/**
 * @brief Checks if the sensor is ready
 *
 * @return true Sensor is ready
 * @return false Sensor is not ready
 */
bool bme680_sensor_is_ready(void);

/**
 * @brief Deinitializes the sensor and frees resources
 */
void bme680_sensor_deinit(void);

/**
 * @brief Task which has to be started for continuous reading
 */
void bme680_read_task(void *pvParameters);
