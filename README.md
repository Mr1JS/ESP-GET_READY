- In lib add the header file for the sensor
- In the src add your sensor .c file
- init in main and start the sesnor before starting the xcreateTask


---

# Steps FOR USING WIFI

### Importtant --> alway run nvs_flash_init() before connect_wifi()

After wifi is connected :

- mqtt_pubsub_start() 

or 

- mqtt_broker_start() ...


---
IF Certificate need for authentication in mqtt_pubsub then add it in new line:

- platform.ini --> board_build.embed_txtfiles
# AND
- src/CMakeLists.txt --> after EMBED_TXTFILES
# AND
- Pem file in the src/certs folder