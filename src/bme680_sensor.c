#include "bme680_sensor.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ============================ INTERNAL CONSTANTS ============================

/**
 * @brief Tag for logging
 */
static const char *TAG = "BME680_SENSOR";

/**
 * @brief Conversion factor from Pascal to Hectopascal
 */
#define PASCAL_TO_HECTOPASCAL 100.0f

/**
 * @brief Conversion factor from Ohm to Kiloohm
 */
#define OHM_TO_KILOOHM 1000.0f

/**
 * @brief Timeout for I2C operations in ms
 */
#define BME680_I2C_TIMEOUT_MS 1000

// ============================ INTERNAL VARIABLES ============================

/**
 * @brief Static variable for sensor handle
 */
static bme680_handle_t s_bme680_handle = NULL;

/**
 * @brief Initialization status of the sensor
 */
static bool s_sensor_initialized = false;

// ============================ FUNCTION IMPLEMENTATIONS ============================

esp_err_t bme680_sensor_init(void)
{
    ESP_LOGI(TAG, "Initializing BME680 sensor...");

    // Check if I2C bus is already initialized
    i2c_master_bus_handle_t bus_handle = i2c_bus_get_handle();
    if (bus_handle == NULL)
    {
        ESP_LOGE(TAG, "I2C bus is not initialized!");
        return ESP_FAIL;
    }

    // Prepare BME680 configuration
    bme680_config_t bme_config = I2C_BME680_CONFIG_DEFAULT;
    bme_config.i2c_address = BME680_I2C_ADDRESS;
    // bme_config.standby_time = BME680_I2C_TIMEOUT_MS;

    // Initialize sensor with retry mechanism
    esp_err_t ret = ESP_FAIL;

    for (int attempt = 1; attempt <= BME680_MAX_INIT_RETRIES; attempt++)
    {
        ESP_LOGI(TAG, "Initialization attempt %d/%d...", attempt, BME680_MAX_INIT_RETRIES);

        ret = bme680_init(bus_handle, &bme_config, &s_bme680_handle);

        if (ret == ESP_OK && s_bme680_handle != NULL)
        {
            s_sensor_initialized = true;
            ESP_LOGI(TAG, "BME680 sensor successfully initialized (Address: 0x%02X)",
                     BME680_I2C_ADDRESS);
            return ESP_OK;
        }

        if (attempt < BME680_MAX_INIT_RETRIES)
        {
            ESP_LOGW(TAG, "Initialization failed, retrying in %dms...",
                     BME680_INIT_RETRY_DELAY_MS);
            vTaskDelay(pdMS_TO_TICKS(BME680_INIT_RETRY_DELAY_MS));
        }
    }

    ESP_LOGE(TAG, "BME680 initialization finally failed after %d attempts: %s",
             BME680_MAX_INIT_RETRIES, esp_err_to_name(ret));
    return ret;
}

esp_err_t bme680_sensor_start_measurement(void)
{
    if (!bme680_sensor_is_ready())
    {
        ESP_LOGE(TAG, "Sensor not initialized!");
        return ESP_FAIL;
    }

    // Advanced settings can be configured here
    // Example: Set heater profile if needed

    ESP_LOGI(TAG, "Measurement started");
    return ESP_OK;
}

esp_err_t bme680_sensor_read_data(float *temperature, float *humidity,
                                  float *pressure, float *gas_resistance)
{
    if (!bme680_sensor_is_ready())
    {
        ESP_LOGE(TAG, "Sensor not initialized!");
        return ESP_FAIL;
    }

    // Read sensor data
    bme680_data_t sensor_data;
    esp_err_t ret = bme680_get_data(s_bme680_handle, &sensor_data);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error reading sensor data: %s", esp_err_to_name(ret));
        return ret;
    }

    // Write data to the provided variables
    if (temperature != NULL)
    {
        *temperature = sensor_data.air_temperature;
    }

    if (humidity != NULL)
    {
        *humidity = sensor_data.relative_humidity;
    }

    if (pressure != NULL)
    {
        *pressure = sensor_data.barometric_pressure / PASCAL_TO_HECTOPASCAL;
    }

    if (gas_resistance != NULL)
    {
        *gas_resistance = sensor_data.gas_resistance / OHM_TO_KILOOHM;
    }

    return ESP_OK;
}

bme680_handle_t bme680_sensor_get_handle(void)
{
    return s_bme680_handle;
}

bool bme680_sensor_is_ready(void)
{
    return s_sensor_initialized && (s_bme680_handle != NULL);
}

void bme680_sensor_deinit(void)
{
    if (s_bme680_handle != NULL)
    {
        bme680_delete(s_bme680_handle);
        s_bme680_handle = NULL;
        s_sensor_initialized = false;
        ESP_LOGI(TAG, "BME680 sensor deinitialized");
    }
}

/**
 * @brief Task for periodic sensor reading
 */
void bme680_read_task(void *pvParameters)
{
    ESP_LOGI(TAG, "BME680 read task started (Interval: %dms)",
             BME680_MEASUREMENT_INTERVAL_MS);

    float temperature, humidity, pressure, gas_resistance;

    while (1)
    {
        // Read sensor data
        esp_err_t ret = bme680_sensor_read_data(&temperature, &humidity,
                                                &pressure, &gas_resistance);

        if (ret == ESP_OK)
        {
            // Data successfully read - output
            ESP_LOGI(TAG, "=== BME680 SENSOR VALUES ===");
            ESP_LOGI(TAG, "Temperature:      %.2f °C", temperature);
            ESP_LOGI(TAG, "Humidity:         %.2f %%", humidity);
            ESP_LOGI(TAG, "Pressure:         %.2f hPa", pressure);
            ESP_LOGI(TAG, "Gas Resistance:   %.2f kΩ", gas_resistance);
            ESP_LOGI(TAG, "=============================");
        }
        else
        {
            ESP_LOGE(TAG, "Error reading sensor data!");
        }

        // Wait until next measurement (configurable)
        vTaskDelay(pdMS_TO_TICKS(BME680_MEASUREMENT_INTERVAL_MS)); // after 5 seconds
    }
}