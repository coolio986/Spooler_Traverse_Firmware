/*
* Serial_Commands.cpp
*
* Created: 3/13/2019 3:35:39 PM
* Author: Anthony
*/


#include "Serial_Commands.h"
#include <Arduino.h>
#include "Serial_Processing.h"


CMD_STR(home, "");
CMD_STR(test, "");

sCommand Cmds[] =
{
  COMMAND(home),
  COMMAND(test),
  {"",0 }

};

static int home_cmd(int argc, char str[32])
{

  Serial.println("Homing");
  Serial.println(str);
  //uint32_t i;
  //if (argc>=1)
  //{
  //  i=atol(argv[0]);
  //  SysLogDebug(i);
  //}
}

static int test_cmd(int argc, char str[32])
{

  Serial.println("testing");
  Serial.println(str);
  //uint32_t i;
  //if (argc>=1)
  //{
  //  i=atol(argv[0]);
  //  SysLogDebug(i);
  //}
}




int Serial_Commands::commandsProcess(void)
{

  Serial_Processing _Serial_Processing;

  _Serial_Processing.CommandsProcess(Cmds);


  
}



// default constructor
Serial_Commands::Serial_Commands()
{
} //Serial_Commands

// default destructor
Serial_Commands::~Serial_Commands()
{
} //~Serial_Commands
