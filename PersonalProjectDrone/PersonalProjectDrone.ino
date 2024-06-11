#include <Servo.h>
#include <Bounce2.h>

#define MeterPin A5
#define EscPin 8
#define ButtonPin 3

#define PulseMin 1000
#define PulseMax 2000
#define PulseBrake 1000


int throttle = PulseMin;
bool startReading = false; //default off -> not reading

Servo ESC; //ESC functioning as same as Servo (Servo class from Servo library)
Bounce ButtonStart = Bounce(); //Bounce from library Bounce2.h

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(ButtonPin, INPUT_PULLUP); //setup for button pin
  ButtonStart.attach(ButtonPin);
  ButtonStart.interval(5); //set timing merging 5mili
  ESC.attach(EscPin);
  ESC.writeMicroseconds(PulseMin);

//sending signal that the Arduino is lock and load by blinking
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);


}

void loop() {
  ButtonStart.update(); //update for the button is start
  if(ButtonStart.fell()) //button is pressed
  { 
    if(!startReading) //conditional not in reading mode
    {
      startReading = true; //change to reading mode 
      digitalWrite(LED_BUILTIN, HIGH); //signal that the start sensor is reading
    }
    else //conditional in reading mode
    {
      ESC.writeMicroseconds(PulseBrake); //brake motors
      startReading = false; //change to stop reading
      throttle = PulseBrake;
      digitalWrite(LED_BUILTIN, LOW); //signal that the sensor read is off
    }

  }


  if(startReading) //in reading mode
  {
    int sensorValue = analogRead(MeterPin); // update reading from potentiometer
    int newThrottle = map(sensorValue, 0, 1023, PulseMin, PulseMax);
    int ThrottleDiff = abs(newThrottle - throttle);
    
    if (ThrottleDiff > 5) //update ESC when the reading diff by 5
    {
      throttle = newThrottle;
      ESC.writeMicroseconds(throttle); //update ESC
    }
  }
}
  