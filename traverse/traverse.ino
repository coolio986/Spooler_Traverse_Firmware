/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <math.h>
#include "ADC_Configuration.h"
#include "Step_Output_Configuration.h"
#include "Serial_Processing.h"
#include "Math_Helpers.h"
#include "Serial_Commands.h"
#include "Globals.h"


//WHEN DEBUGGING MAKE SURE TO SELECT GDB UNDER traverse->Properties->Advanced->check GDB

/*End of auto generated code by Atmel studio */

void CheckDirection(); //function prototype
void ToggleStepOutput();


const int stepPin = 3;
const int potPin = 0;
volatile const int directionInput = 6;
volatile const int directionOutputPin = 4;
volatile const int reversingCountMax = 1000;

int potValue = 0;
volatile long stepDelay = 0;
volatile bool directionInputState = false;
volatile bool directionOutputState = false;

uint32_t serialTimerMillis = 1000; //Serial output scheduler delay time
uint32_t serialTimerPreviousMillis = 0; //Current delay time
volatile uint32_t reversingCounts = 0;

int STEPS = 0;
bool RUN = true;
bool HOME = false;
volatile uint32_t STEPRATE = 0;


#ifdef USE_POT_FOR_TRAVERSE
volatile bool potEnabled = true;
#else
volatile bool potEnabled = false;
#endif

Serial_Commands _Serial_Commands;
Math_Helpers _Math_Helpers;


ISR (TIMER1_OVF_vect){


  if (RUN)
  {
    if (directionInputState){ STEPS++; }
    if (!directionInputState){ STEPS--; }
    TCNT1 = stepDelay;
    
    #ifdef USE_POT_FOR_TRAVERSE
    if (potEnabled){ ToggleStepOutput();}
    #else
    ToggleStepOutput();
    #endif

    if (directionOutputState != directionInputState) //direction changed state
    {
      reversingCounts++;

      if (reversingCounts >= reversingCountMax)
      {
        CheckDirection();
      }
    }
  }
  if (HOME)
  {
    #ifdef USE_POT_FOR_TRAVERSE
    potEnabled = false;
    #endif
    TCNT1 = 60000;
    ToggleStepOutput();
    PORTD &= ~digitalPinToBitMask(directionOutputPin);

    if (!HOMING_SWITCH_USES_SPRING_RETURN){
      if (directionInputState == HIGH)
      {
        reversingCounts++;

        if (reversingCounts >= reversingCountMax)
        {
          HOME = false;
          STEPS = 0;
          RUN = true;
          
          #ifdef USE_POT_FOR_TRAVERSE
          potEnabled = true;
          #endif
          
          CheckDirection();
        }
      }
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
  
  _Serial_Commands.commandsProcess();

  directionInputState = PIND & 0x20; //0x40 = pin 5 (0010 0000) //read pot
  
  if (potEnabled)
  {
    potValue = analogRead(potPin);
    
    stepDelay = _Math_Helpers.fscale(ADC_MIN_VALUE, ADC_MAX_VALUE, 0, 65000, potValue, 40);
  }
  else
  {
    RUN = STEPRATE != 0;
    stepDelay = _Math_Helpers.fscale(ADC_MIN_VALUE, ADC_MAX_VALUE, 0, 65000, STEPRATE, 40);
  }
  
}


void CheckDirection()
{
  if (directionInputState == LOW){
    PORTD &= ~digitalPinToBitMask(directionOutputPin);
  }
  else{
    PORTD |= digitalPinToBitMask(directionOutputPin);
  }

  directionOutputState = directionInputState;
  reversingCounts = 0;
  
}

void ToggleStepOutput()
{
  PORTD ^= !(PIND & 0x08) | 0x08;
}
