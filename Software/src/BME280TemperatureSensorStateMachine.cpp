#include "BME280TemperatureSensorStateMachine.h"

#include <Wire.h>

void BME280TemperatureSensorStateMachine::init() {
  Wire.begin();
  bme280.setI2CAddress(0x76);
  if (bme280.beginI2C() == false)  // Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1)
      ;  // Freeze
  }

  bme280.setMode(MODE_SLEEP); //Sleep for now
}

void BME280TemperatureSensorStateMachine::startSampling() {
  Serial.println(F("Requesting Temperature from BME280."));
  bme280.setMode(MODE_FORCED);

   long startTime = millis();
  while(bme280.isMeasuring() == false) ; //Wait for sensor to start measurment
  while(bme280.isMeasuring() == true) ; //Hang out while sensor completes the reading    
  long endTime = millis();

  //Sensor is now back asleep but we get get the data

  Serial.print(" Measure time(ms): ");
  Serial.print(endTime - startTime);

  Serial.print(" Humidity: ");
  Serial.print(bme280.readFloatHumidity(), 0);

  Serial.print(" Pressure: ");
  Serial.print(bme280.readFloatPressure(), 0);

  Serial.print(" Alt: ");
  Serial.print(bme280.readFloatAltitudeMeters(), 1);
  //Serial.print(bme280.readFloatAltitudeFeet(), 1);

  float temp = bme280.readTempC();
  value = temp;

  Serial.print(" Temp: ");
  Serial.print(temp, 2);
  //Serial.print(bme280.readTempF(), 2);

  Serial.println();

  this->state == SensorValueStatus::kSampleAvailable;
}
