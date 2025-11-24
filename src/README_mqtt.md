Standard port --> 1883


SUBSCRIBE
mosquitto_sub -h localhost -t esp32/values


SEND MESSAGE
mosquitto_pub -h 192.168.178.102 -t esp32/values -m "Hello ESP32-1.2"









; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:lolin_d32_pro]
platform = espressif32
board = lolin_d32_pro
framework = espidf
monitor_speed = 115200
lib_extra_dirs = lib/

; For the certificates:
board_build.embed_txtfiles = 
  src/certs/isrg-root-x1.pem