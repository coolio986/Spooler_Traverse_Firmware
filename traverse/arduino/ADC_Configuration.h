/* 
* ADC_Configuration.h
*
* Created: 3/12/2019 3:57:23 PM
* Author: Anthony
*/


#ifndef __ADC_CONFIGURATION_H__
#define __ADC_CONFIGURATION_H__


#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif




class ADC_Configuration
{
//variables
public:
protected:
private:

//functions
public:
  ADC_Configuration();
  ~ADC_Configuration();
  static void SetupADC(void);
protected:
private:
  ADC_Configuration( const ADC_Configuration &c );
  ADC_Configuration& operator=( const ADC_Configuration &c );

}; //ADC_Configuration

#endif //__ADC_CONFIGURATION_H__
