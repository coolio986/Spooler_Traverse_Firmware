/*
* Serial_Processing.h
*
* Created: 3/12/2019 4:43:35 PM
* Author: Anthony
*/


#ifndef __SERIAL_PROCESSING_H__
#define __SERIAL_PROCESSING_H__

#include <Arduino.h>
#include "HardwareTypes.h"
#include "Serial_Commands.h"

#define SERIAL_BAUD (115200) //baud rate for the serial ports




const byte numChars = 32;
static char receivedChars[numChars]; // an array to store the received data

typedef struct
{
  uint16_t hardwareType; //see hardwareTypes.h
  char *value;
  
  
} serialCommand;






class Serial_Processing
{
  //variables
  public:
  protected:
  private:
  byte computer_bytes_received = 0;    //We need to know how many characters bytes have been received
  byte sensor_bytes_received = 0;      //We need to know how many characters bytes have been received
  //const static byte numberOfBufferBytes = 32;
  char computerdata[MAX_CMD_LENGTH] = {0};               //A 20 byte character array to hold incoming data from a pc/mac/other
  char sensordata[MAX_CMD_LENGTH] = {0};                  //A 30 byte character array to hold incoming data from the sensors
  char *hardwareType;                       //Char pointer used in string parsing
  char *cmd;                           //Char pointer used in string parsing
  char *arguments;          //Char pointer used in string parsing
  

  //functions
  public:
  Serial_Processing();
  ~Serial_Processing();

  void Setup(void);
  
  unsigned int CommandParse(sCommand *ptrCmds, char str[MAX_CMD_LENGTH]);
  int CommandsProcess(sCommand *ptrCmds);


  protected:
  private:
  Serial_Processing( const Serial_Processing &c );
  Serial_Processing& operator=( const Serial_Processing &c );
  int strcicmp(char const *a, char const *b);
  
  

  



}; //Serial_Processing

#endif //__SERIAL_PROCESSING_H__
