#include "GyroSensor.h"
#include <Wire.h>
#include <math.h>

float RatePitch, RateYaw, RateRoll;
float x, y, z;
float AngleRoll, AnglePitch;

void gyro_signals() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // Accelerometer data
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);

  if (Wire.available() == 6) {
    int16_t AccXLSB = Wire.read() << 8 | Wire.read();
    int16_t AccYLSB = Wire.read() << 8 | Wire.read();
    int16_t AccZLSB = Wire.read() << 8 | Wire.read();

    x = (float)AccXLSB / 4096.0 - 0.06;
    y = (float)AccYLSB / 4096.0 - 0.02;
    z = (float)AccZLSB / 4096.0 - 0.18;

    AngleRoll = atan2(y, sqrt(x * x + z * z)) * 180 / PI;
    AnglePitch = -atan2(x, sqrt(y * y + z * z)) * 180 / PI;
  }

  Wire.beginTransmission(0x68);
  Wire.write(0x43);  // Gyroscope data
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);

  if (Wire.available() == 6) {
    int16_t GyroX = Wire.read() << 8 | Wire.read();
    int16_t GyroY = Wire.read() << 8 | Wire.read();
    int16_t GyroZ = Wire.read() << 8 | Wire.read();

    RateRoll = (float)GyroX / 65.5;
    RatePitch = (float)GyroY / 65.5;
    RateYaw = (float)GyroZ / 65.5;
  }
}

void MpuSetup() {
  Wire.begin();
  Wire.setClock(400000);
  delay(100);

  Wire.beginTransmission(0x68);  // Wake up MPU6050
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void GyroSensor(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    gyro_signals();
    Serial.print("Roll: ");
    Serial.print(AngleRoll);
    Serial.print(" | Pitch: ");
    Serial.print(AnglePitch);
    Serial.print(" | Yaw rate: ");
    Serial.println(RateYaw);
    vTaskDelay(pdMS_TO_TICKS(100));  // Run every 100 ms
  }
}