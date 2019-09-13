/*
* ADC_Configuration.cpp
*
* Created: 3/12/2019 3:57:23 PM
* Author: Anthony
*/

#include <Arduino.h>
#include "ADC_Configuration.h"

// default constructor
ADC_Configuration::ADC_Configuration()
{
  
} //ADC_Configuration

void ADC_Configuration::SetupADC(void)
  {
    #if FASTADC
    // set prescale to 16
    sbi(ADCSRA,ADPS2) ;
    cbi(ADCSRA,ADPS1) ;
    cbi(ADCSRA,ADPS0) ;
    #endif
  }


// default destructor
ADC_Configuration::~ADC_Configuration()
{
} //~ADC_Configuration
