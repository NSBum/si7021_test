// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C driver
#include "driver/i2c.h"

// Error library
#include "esp_err.h"

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include <stdio.h>

#include "si7021.h"

#define I2C_SDA	21	//	GPIO_NUM_21
#define I2C_SCL 22	//	GPIO_NUM_22

void query_sensor_task(void *pvParameter) {
    while(1) {
        float temp = si7021_read_temperature();
        float hum = si7021_read_humidity();
        
        printf("%0.2f degrees C, %0.2f%% RH\n", temp, hum);
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

//	application entry point
int app_main(void) {
	//Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

	//	initialize I2C driver/device
    ret = si7021_init(I2C_NUM_0, I2C_SDA, I2C_SCL,
    	GPIO_PULLUP_DISABLE,GPIO_PULLUP_DISABLE);
    ESP_ERROR_CHECK(ret);
    printf("I2C driver initialized\n");

    xTaskCreate(&query_sensor_task,
        "sensor_task",
        2048,
        NULL,
        5,
        NULL);
        return 0;
}
