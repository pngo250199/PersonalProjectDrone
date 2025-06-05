#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#ifndef TASK2_H
#define TASK2_H

void TaskBlink1(void *pvParameters);
 extern volatile bool led2State;
#endif