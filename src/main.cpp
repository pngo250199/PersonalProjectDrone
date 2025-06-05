#include <Arduino.h>
#include <WiFi.h>
#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Task1.h"  // Include the header file for TaskBlink
#include "Task2.h"
#include "GyroSensor.h"
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>

const char *ssid = "ATTQSpshJw";
const char *password = "e7khgbpcybwi";

WebServer server(80);

volatile bool led1State = false;
volatile bool led2State = false;

#if CONFIG_FREERTOS_UNICORE
#define TASK_RUNNING_CORE 0
#else
#define TASK_RUNNING_CORE 1
#endif

void handleRoot();
void TaskWebServer(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
TaskHandle_t analog_read_task_handle;  // You can (don't have to) use this to be able to manipulate a task from somewhere else.
TaskHandle_t TaskHandle;

String generatePage() {
  File file = SPIFFS.open("/monitor.html", "r");
  if (!file) {
    return "Failed to open HTML file";
  }

  String htmlContent;
  while (file.available()) {
    htmlContent += file.readString();
  }
  file.close();
  return htmlContent;
}

if(!SPIFFS.begin())
{
  return "cannot open";

}

server.on("/monitor.html", [](AsyncWebServerRequest *request) {
  String htmlContent = generatePage();
  request->send(200, "text/html", htmlContent);
});



void TaskWebServer(void *pvParameters) {
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  while (true) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}


// The setup function runs once when you press reset or power on the board.
void setup() {


  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  delay(1000);


  // WiFi setup
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());


  //MpuSetup();
  // Set up two tasks to run independently.
  uint32_t blink_delay = 1000;  // Delay between changing state on LED pin
  uint32_t blink_delay1 = 1000;
  xTaskCreate(
    TaskBlink, "Task Blink"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&blink_delay  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );
  // This variant of task creation can also specify on which core it will be run (only relevant for multi-core ESPs)

xTaskCreate(
    TaskBlink1, "Task Blink1"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&blink_delay1  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );

/*xTaskCreate(
  GyroSensor, "Gyro Sensor Task",  // A name just for humans,
  2048,  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
  (void*) &TaskHandle,
  1,
  NULL
);
*/

 xTaskCreatePinnedToCore(
    TaskWebServer,
    "Task WebServer",
    4096,
    NULL,
    1,
    NULL,
    TASK_RUNNING_CORE);

}


void loop() {
  if (analog_read_task_handle != NULL) {  // Make sure that the task actually exists
    delay(10000);
    vTaskDelete(analog_read_task_handle);  // Delete task
    analog_read_task_handle = NULL;        // prevent calling vTaskDelete on non-existing task
  }
}




