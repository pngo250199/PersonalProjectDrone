#include "Task2.h"

#ifndef LED_BUILTIN1
#define LED_BUILTIN1 4  // Specify the on which is your LED
#endif

void TaskBlink1(void *pvParameters) {  // This is a task.
  uint32_t blink_delay1 = *((uint32_t *)pvParameters);
 
  pinMode(LED_BUILTIN1, OUTPUT);

  for (;;) {                          // A Task shall never return or exit.
    digitalWrite(LED_BUILTIN1, HIGH);  // turn the LED on (HIGH is the voltage level)
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    Serial.printf("TaskBlink1 is turn on on core %d\n", xPortGetCoreID());
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    vTaskDelay(blink_delay1);
    digitalWrite(LED_BUILTIN1, LOW);  // turn the LED off by making the voltage LOW
    Serial.printf("TaskBlink1 is turn off on core %d\n", xPortGetCoreID());
    vTaskDelay(blink_delay1);

    while(true)
    {
      led2State =!led2State;
      digitalWrite(LED_BUILTIN1, led2State);
      vTaskDelay(blink_delay1);
    }
  }


}