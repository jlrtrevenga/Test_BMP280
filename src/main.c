#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include <esp_log.h>
#include "bmp280.h"

// Pasarlos a parametros para que llegen desde el bucle principal, 
// puesto que pueden existir otros sensores que comuniquen por I2C y los pines deben ser comunes.
#define SDA_GPIO 21
#define SCL_GPIO 22

static const char *TAG = "BMP280_READ_LOOP";

/**
 * @brief read BMP280/BME280 values via I2C protocol and update measured values.
 * @brief FORCED MODE operation is used instead of NORMAL MODE to handle manually loop read period
 * @brief TODO: Add signals to handle coordination: START, STOP, ETC.
 * @param[out] dev pointer to mesured values: temperature, pressure, (humidity)
 * @param[in] loop_period BMP280/BME280 read period
 * @param[in] port I2C port number
 * @param[in] sda_gpio GPIO pin for SDA
 * @param[in] scl_gpio GPIO pin for SCL
 * @return `ESP_OK` on success
 */
void bmp280_read_loop(void *pvParameters) {
    // Receive loop period and measures structures from main loop
    int sda_gpio = xx21;
    int scl_gpio = xx22;
    int loop_period = xx;           //  Loop period in ms.

    bmp280_params_t params;
    bmp280_t dev;
    memset(&dev, 0, sizeof(bmp280_t));

    ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, sda_gpio, scl_gpio));     // check if "port" should be received as parameter
    bmp280_init_forced_default_params(&params);  
    ESP_ERROR_CHECK(bmp280_init(&dev, &params));

    bool bme280p = dev.id == BME280_CHIP_ID;
    printf("BMP280: found %s\n", bme280p ? "BME280" : "BMP280");

    float pressure, temperature, humidity;
    while (1) {
        vTaskDelay(loop_period / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(bmp280_force_measurement(&dev));
        if (bmp280_read_float(&dev, &temperature, &pressure, &humidity) == ESP_OK) {
            // pasa los valores a las medidas. quality = GOOD_QUALITY/MED_QUALITY
            printf("Pressure: %.2f Pa, Temperature: %.2f C", pressure, temperature);
            if (bme280p)
                printf(", Humidity: %.2f\n", humidity);
            else
                printf("\n");
            } 
        else {
            // corrige el valor de quality para que pueda procesarlo en caso de error de lectura
            // quality = BAD_QUALITY
            printf("Temperature/pressure reading failed\n");
            continue;            
            }
        }
    }



void app_main()
{  
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(bmp280_read_loop, "bmp280_read_loop", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}

