#pragma once
#include "driver/i2c_master.h"
#include "esp_err.h"

// ---- Configurations ----
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_PORT I2C_NUM_0

esp_err_t i2c_bus_init(void);

i2c_master_bus_handle_t i2c_bus_get_handle(void);

esp_err_t i2c_bus_add_device(uint8_t address,
                             uint32_t clk_speed,
                             i2c_master_dev_handle_t *out_dev_handle);
