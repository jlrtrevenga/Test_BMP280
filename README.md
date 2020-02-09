# Test_BMP280
BPM_280 Test loop

ESP32 - ESP_IDF & Visual Code

MODES: 
NORMAL MODE operation test
FORCED MODE operation test

## Pending
Create a library to encapsulate read loop, including all parameters:

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
