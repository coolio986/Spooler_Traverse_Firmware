/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <math.h>
#include "ADC_Configuration.h"
#include "Step_Output_Configuration.h"
#include "Serial_Processing.h"
#include "Math_Helpers.h"

/*End of auto generated code by Atmel studio */


const int stepPin = 3;
const int potPin = 0;
const int directionInput = 6;
const int directionOutputPin = 4;
const int reversingCountMax = 1000;

int potValue = 0;
long stepDelay = 0;
bool directionInputState = false;
bool directionOutputState = false;
bool stepOutputState = false;


uint32_t serialTimerMillis = 1000; //Serial output scheduler delay time
uint32_t serialTimerPreviousMillis = 0; //Current delay time
int32_t stepCounter = 0;
uint32_t reversingCounts = 0;

Serial_Processing _Serial_Processing;
Math_Helpers _Math_Helpers;


ISR (TIMER1_OVF_vect){

  if (directionInputState){ stepCounter++; }
  if (!directionInputState){ stepCounter--; }
  TCNT1 = stepDelay;
  
  PORTD ^= !(PIND & 0x08) | 0x08;

  if (directionOutputState != directionInputState) //direction changed state
  {
    reversingCounts++;

    if (reversingCounts >= reversingCountMax){
      if (directionInputState == LOW){
        PORTD &= ~digitalPinToBitMask(directionOutputPin);
      }
      else{
        PORTD |= digitalPinToBitMask(directionOutputPin);
      }

      
      directionOutputState = directionInputState;
      reversingCounts = 0;
    }
  }
  
}

void setup() {
  
  pinMode(stepPin, OUTPUT);
  pinMode(directionOutputPin, OUTPUT);
  pinMode(directionInput, INPUT);

  ADC_Configuration _ADC_Configuration ;
  Step_Output_Configuration _Step_Output_Configuration;

  _ADC_Configuration.SetupADC();
  _Step_Output_Configuration.SetupStepOutput();

  Serial.begin(115200);

  sei(); // Enable global interrupts
  
}

void loop() {
  
  
  _Serial_Processing.RunSerialDataLoop();
  
  directionInputState = PIND & 0x20; //0x40 = pin 5 (0010 0000)
  
  potValue = analogRead(potPin);
  
  stepDelay = _Math_Helpers.fscale(0, 1023, 0, 65000, potValue, 40); //this uses log math to determine sweep speed
  
  //if (millis() - serialTimerPreviousMillis >= serialTimerMillis) //Update rate of serial output
  //{
  //
  //Serial.println("_____________________");
  ////Serial.println(stepDelay);
  ////Serial.println(stepDelayMicros);
  //Serial.println(stepCounter);
  //Serial.println(directionOutputState);
  ////Serial.println("_____________________");
  //serialTimerPreviousMillis = millis();
  //}
  
}
