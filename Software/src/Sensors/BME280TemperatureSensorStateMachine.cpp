#include "BME280TemperatureSensorStateMachine.h"

#include <Arduino.h>
#include <Wire.h>

#include "bme280.h"

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs.
 *
 *  @param[in] period  : The required wait time in microseconds.
 *  @return void.
 *
 */
void user_delay_ms(uint32_t period) { delay(period); }

void bmeErrorCheck(int8_t result, const char *text = "") {
  if (result != BME280_OK) {
    Serial.print(F("Error '"));
    Serial.print(result, DEC);
    Serial.print(F("' during BMW280 transaction: "));
    Serial.println(text);
    while (1)
      ;
  }
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
                     uint16_t len) {
  int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

  Wire.beginTransmission(dev_id);
  Wire.write(reg_addr);
  rslt = Wire.endTransmission();

  if (rslt == 0) {
    Wire.requestFrom(dev_id, len);
    Wire.readBytes(reg_data, len);
    rslt = Wire.endTransmission();
  }

  /*
   * The parameter dev_id can be used as a variable to store the I2C address of
   * the device
   */

  /*
   * Data on the bus should be like
   * |------------+---------------------|
   * | I2C action | Data                |
   * |------------+---------------------|
   * | Start      | -                   |
   * | Write      | (reg_addr)          |
   * | Stop       | -                   |
   * | Start      | -                   |
   * | Read       | (reg_data[0])       |
   * | Read       | (....)              |
   * | Read       | (reg_data[len - 1]) |
   * | Stop       | -                   |
   * |------------+---------------------|
   */

  return rslt;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
                      uint16_t len) {
  int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

  /*
   * The parameter dev_id can be used as a variable to store the I2C address of
   * the device
   */

  Wire.beginTransmission(dev_id);
  Wire.write(reg_addr);
  Wire.write(reg_data, len);
  rslt = Wire.endTransmission();

  /*
   * Data on the bus should be like
   * |------------+---------------------|
   * | I2C action | Data                |
   * |------------+---------------------|
   * | Start      | -                   |
   * | Write      | (reg_addr)          |
   * | Write      | (reg_data[0])       |
   * | Write      | (....)              |
   * | Write      | (reg_data[len - 1]) |
   * | Stop       | -                   |
   * |------------+---------------------|
   */

  return rslt;
}

void BME280TemperatureSensorStateMachine::init() {
  Wire.begin();

  dev.dev_id = BME280_I2C_ADDR_PRIM;

  dev.delay_ms = user_delay_ms;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.intf = BME280_I2C_INTF;

  bmeErrorCheck(bme280_init(&dev), "bme280_init");

  bmeErrorCheck(bme280_get_sensor_settings(&dev), "bme280_get_sensor_settings");
  dev.settings.osr_h = BME280_OVERSAMPLING_1X;
  dev.settings.osr_p = BME280_OVERSAMPLING_1X;
  dev.settings.osr_t = BME280_OVERSAMPLING_1X;
  dev.settings.filter = BME280_FILTER_COEFF_OFF;
  bmeErrorCheck(bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, &dev),
                "bme280_set_sensor_settings");

  measurementDelay = bme280_cal_meas_delay(&dev.settings);

  bmeErrorCheck(bme280_set_sensor_mode(BME280_SLEEP_MODE, &dev),
                "bme280_set_sensor_mode");
}

void BME280TemperatureSensorStateMachine::startSampling() {
  Serial.println(F("Requesting Measurements from BME280."));
  bmeErrorCheck(bme280_set_sensor_mode(BME280_FORCED_MODE, &dev),
                "bme280_set_sensor_mode");

  delay(measurementDelay);

  bme280_data comp_data;
  bmeErrorCheck(bme280_get_sensor_data(BME280_ALL, &comp_data, &dev),
                "bme280_get_sensor_data");

  // Tmperature in 100th oC
  // 1024ths % relative humidity
  // pascal

  /*

  long startTime = millis();
  while (bme280.isMeasuring() == false)
    ;  // Wait for sensor to start measurment
  while (bme280.isMeasuring() == true)
    ;  // Hang out while sensor completes the reading
  long endTime = millis();
*/
  // Sensor is now back asleep but we get get the data

  // Serial.print(" Measure time(ms): ");
  // Serial.print(endTime - startTime);

  Serial.print(F(" Humidity: "));
  Serial.print(comp_data.humidity / 1024, DEC);

  Serial.print(F(" Pressure: "));
  Serial.print(comp_data.pressure, DEC);

  /*
    Serial.print(" Alt: ");
    Serial.print(bme280.readFloatAltitudeMeters(), 1);
    // Serial.print(bme280.readFloatAltitudeFeet(), 1);

    float temp = bme280.readTempC();
    value = temp;
  */
  Serial.print(F(" Temp: "));
  float temperature = comp_data.temperature / 100.0f;
  Serial.print(temperature, DEC);
  // Serial.print(bme280.readTempF(), 2);

  // comp_data.temperature is given in 100ths of degrees
  // we want to TX hunders of degrees -> matches!
  // But we need a sign-perfect narrowing conversion.

  value = comp_data.temperature;

  Serial.println();

  this->state = SensorValueStatus::kSampleAvailable;
}
