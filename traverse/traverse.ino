/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include <PWM.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

const int stepPin = 3;
const int potPin = 0;
const int directionInput = 6;
const int directionOutputPin = 4;

int potValue = 0;
int directionDelay = 0;
int previousDirectionMillis = 0;

int32_t outputFrequency = 0;
int32_t maxOutputFrequency = 10666;
int frequencyOutputDelay = 20;
int previousOutputMillis= 0;
bool directionInputState = false;
bool directionOutputState = false;

void setup() {
  // put your setup code here, to run once:
  InitTimersSafe(); //Setup Pwm.h to allow safe timers
  SetPinFrequencySafe(stepPin, outputFrequency); //Setup output pin for safe timers
  
  pinMode(stepPin, OUTPUT);
  pinMode(directionOutputPin, OUTPUT);
  pinMode(directionInput, INPUT);
  
  Serial.begin(115200);
}

void loop() {
  directionDelay = (maxOutputFrequency / outputFrequency) * 10;
  directionInputState = digitalRead(directionInput);
  
  if (millis() - previousDirectionMillis >= directionDelay && directionInputState != directionOutputState) {
    directionOutputState = !directionOutputState;
    digitalWrite(directionOutputPin, directionOutputState);
    previousDirectionMillis = millis();
  }

  if (millis() - previousOutputMillis >= frequencyOutputDelay){
    
    potValue = analogRead(potPin);
    //Serial.println("___________");
    //Serial.print("Pot: ");
    //Serial.println(knobValue1);
    outputFrequency = map(potValue, 0, 1023, 52, maxOutputFrequency);
    //Serial.print("Freq: ");
    //Serial.println(frequency);
    //Serial.println("___________");
    bool success = SetPinFrequencySafe(9,outputFrequency);
    if (success){
      //Serial.println("freq set success");
      pwmWrite(stepPin, 128);
    }
    previousOutputMillis = millis();
  }
  
}
