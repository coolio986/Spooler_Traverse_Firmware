/* 
* globals.h
*
* Created: 3/14/2019 5:26:20 PM
* Author: Anthony
*/
#include "Arduino.h"

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define DELIMITER ";"



extern long STEPS;
extern volatile uint32_t STEPRATE;
extern bool RUN;
extern bool HOME;
extern volatile int32_t DESIRED_POSITION;

#endif //__GLOBALS_H__
