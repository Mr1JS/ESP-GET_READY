// ---- OLED DISPLAY -----
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ssd1306.h>

#define I2C_SDA 21 // SERIAL DATA LINE
#define I2C_SCL 22 // SERIAL CLOCK LINE

/* void app_main(void)
{
    // Display initialisieren (Standard-Adresse 0x3C)
    ssd1306_128x64_i2c_initEx(I2C_SCL, I2C_SDA, 0);
    ssd1306_clearScreen();
    ssd1306_setFixedFont(ssd1306xled_font8x16);

    // Bildschirm löschen
    ssd1306_clearScreen();

    // Text ausgeben
    ssd1306_printFixed(0, 10, "Hello World!", STYLE_NORMAL);
} */