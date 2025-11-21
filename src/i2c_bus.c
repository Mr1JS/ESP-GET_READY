#include "i2c_bus.h"
#include "esp_log.h"

static const char *TAG = "i2c_bus";

static i2c_master_bus_handle_t s_i2c_bus = NULL; // at the begging the master is null

//
// Initialize I2C master bus
//
esp_err_t i2c_bus_init(void)
{
    i2c_master_bus_config_t cfg = {
        .i2c_port = I2C_MASTER_PORT,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = {
            .enable_internal_pullup = true}};

    esp_err_t ret = i2c_new_master_bus(&cfg, &s_i2c_bus);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create I2C bus (%s)", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "I2C bus initialized");
    return ESP_OK;
}

//
// Get the bus handle
//
i2c_master_bus_handle_t i2c_bus_get_handle(void)
{
    return s_i2c_bus;
}

//
// Add a device to the bus
//
esp_err_t i2c_bus_add_device(uint8_t address,
                             uint32_t clk_speed,
                             i2c_master_dev_handle_t *out_dev_handle)
{
    if (s_i2c_bus == NULL)
    {
        ESP_LOGE(TAG, "I2C bus not initialized");
        return ESP_FAIL;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = clk_speed};

    esp_err_t ret = i2c_master_bus_add_device(s_i2c_bus, &dev_cfg, out_dev_handle);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Added I2C device at 0x%02X", address);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to add I2C device 0x%02X (%s)",
                 address, esp_err_to_name(ret));
    }

    return ret;
}
