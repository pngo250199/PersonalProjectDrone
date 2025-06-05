#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#ifndef GYROSENSOR_H
#define GYROSENSOR_H

void GyroSensor(void *pvParameters);
void MpuSetup();
#endif