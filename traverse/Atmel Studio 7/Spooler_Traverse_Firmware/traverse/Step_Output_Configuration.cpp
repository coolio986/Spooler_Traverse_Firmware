/*
* Step_Output_Configuration.cpp
*
* Created: 3/12/2019 4:00:49 PM
* Author: Anthony
*/

#include <Arduino.h>
#include "Step_Output_Configuration.h"


// default constructor
Step_Output_Configuration::Step_Output_Configuration()
{
} //Step_Output_Configuration

void Step_Output_Configuration::SetupStepOutput(void)
{
  //normal mode
  TCCR1A = 0x00;
  OCR1A   = 0;
  TCCR1B = (1<<CS10);
  TIMSK1 |= bit (TOIE1);
}

// default destructor
Step_Output_Configuration::~Step_Output_Configuration()
{
} //~Step_Output_Configuration
