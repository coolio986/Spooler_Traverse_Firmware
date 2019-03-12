/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <math.h>
#include "ADC_Configuration.h"
#include "Step_Output_Configuration.h"
#include "Serial_Processing.h"

/*End of auto generated code by Atmel studio */

#define MYHARDWARETYPE struct_hardwareTypes.traverse

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
  
  stepDelay = fscale(0, 1023, 0, 65000, potValue, 40); //this uses log math to determine sweep speed
  
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

float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
