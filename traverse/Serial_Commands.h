/*
* Serial_Commands.h
*
* Created: 3/13/2019 3:35:39 PM
* Author: Anthony
*/


#ifndef __SERIAL_COMMANDS_H__
#define __SERIAL_COMMANDS_H__

#include <Arduino.h>
#include "Device_Configuration.h"


#define COMMAND(NAME)  { NAME ## _str, NAME ## _cmd}
#define CMD_STR(NAME, STR)  static char NAME ## _str[] = #NAME; static int NAME ##_cmd(int, char str[MAX_CMD_LENGTH]);


typedef struct
{
  char *name;
  int (*function) (int, char str[MAX_CMD_LENGTH]);

} sCommand;




class Serial_Commands
{
  //variables
  public:
  protected:
  private:
  

  //functions
  public:
  Serial_Commands();
  ~Serial_Commands();
  int commandsProcess(void);
  protected:
  private:
  Serial_Commands( const Serial_Commands &c );
  Serial_Commands& operator=( const Serial_Commands &c );

}; //Serial_Commands

#endif //__SERIAL_COMMANDS_H__
