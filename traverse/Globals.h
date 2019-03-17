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



extern int STEPS;
extern volatile uint32_t STEPRATE;
extern bool RUN;
extern bool HOME;

#endif //__GLOBALS_H__
