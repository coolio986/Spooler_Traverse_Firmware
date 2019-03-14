/*
* Serial_Commands.cpp
*
* Created: 3/13/2019 3:35:39 PM
* Author: Anthony
*/


#include "Serial_Commands.h"
#include <Arduino.h>
#include "Serial_Processing.h"
#include "Globals.h"


CMD_STR(home, "");
CMD_STR(getSteps, "");
CMD_STR(status, "");
CMD_STR(stop, "");
CMD_STR(run, "");

sCommand Cmds[] =
{
  COMMAND(home),
  COMMAND(getSteps),
  COMMAND(status),
  COMMAND(stop),
  COMMAND(run),
  {"",0 }

};

static int home_cmd(int argc, char str[MAX_CMD_LENGTH])
{

  Serial.println("Homing Please Wait...");
  RUN = false;
  HOME = true;

  //Serial.println(str);
  //uint32_t i;
  //if (argc>=1)
  //{
  //  i=atol(argv[0]);
  //  SysLogDebug(i);
  //}
}


static int getSteps_cmd(int argc, char str[MAX_CMD_LENGTH])
{

  //Serial.println("get steps");
  Serial.println(STEPS);
  //Serial.println(str);
  //uint32_t i;
  //if (argc>=1)
  //{
  //  i=atol(argv[0]);
  //  SysLogDebug(i);
  //}
}

static int status_cmd(int argc, char str[MAX_CMD_LENGTH])
{
  Serial.print("Motion Controller: ");
  if (RUN)
  {
    Serial.println("Running");
  }
  else
  {
    Serial.println("Stopped");
  }
  
}

static int stop_cmd(int argc, char str[MAX_CMD_LENGTH])
{
  RUN =false;
}

static int run_cmd(int argc, char str[MAX_CMD_LENGTH])
{
  RUN =true;
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
