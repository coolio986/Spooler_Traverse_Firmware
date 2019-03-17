/*
* Serial_Processing.cpp
*
* Created: 3/12/2019 4:43:35 PM
* Author: Anthony
*/

#include <Arduino.h>
#include "Serial_Processing.h"
#include "HardwareTypes.h"
#include "Device_Configuration.h"
#include "Serial_Commands.h"
#include "globals.h"




// default constructor
Serial_Processing::Serial_Processing()
{
} //Serial_Processing


void Serial_Processing::Setup(void)
{
  Serial.begin(SERIAL_BAUD);               //Set the hardware serial port speed
  Serial.setTimeout(50);
}


int Serial_Processing::CommandsProcess(sCommand *ptrCmds)
{
  if (Serial.available() > 0)
  {
    computer_bytes_received = Serial.readBytesUntil(10, computerdata, MAX_CMD_LENGTH); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
    computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
    while(Serial.available()){
    char test = Serial.read(); //flush buffer
    }
  }

  if (computer_bytes_received != 0) {             //If computer_bytes_received does not equal zero
    
    CommandParse(ptrCmds, computerdata);
    
    computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0
  }

  return 0;
}


unsigned int Serial_Processing::CommandParse(sCommand *ptrCmds, char str[MAX_CMD_LENGTH])
{
  hardwareType = strtok(str, DELIMITER); //hardware ID
  cmd = strtok(NULL, DELIMITER);
  arguments = strtok(NULL, DELIMITER);

  for (int i=0; hardwareType[i]!= '\0'; i++)
  {
    //Serial.println(hardwareType[i]);
    if (!isdigit(hardwareType[i]) != 0)
    {
      Serial.println("Invalid Hardware ID, number is not a digit");
      return 0;
    }
  }

  if (MYHARDWARETYPE != atoi(hardwareType))
  {
    Serial.println("Invalid Hardware ID");
    return 0;
  }

  unsigned int i;
  sCommand cmd_list;
  i=0;

  memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));
  
  
  while(cmd_list.function!=0)
  {
    if (strcicmp(cmd,cmd_list.name)==0){
      return (*cmd_list.function)(0,cmd,arguments);
    }

    i=i+1;
    memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));
  }
  
  

  

  //hardwareType = strtok(computerdata, ";");          //Let's parse the string at each colon
  //cmd = strtok(NULL, ";");                      //Let's parse the string at each colon
  ////Open_channel();                               //Call the function "open_channel" to open the correct data path
  //
  //if (cmd != 0) {                               //If a command has been sent
  ////Serial.print(cmd);
  ////Serial.print("\r");
  ////altSerial.print(cmd);                       //Send the command from the computer to the Atlas Scientific device using the softserial port
  ////altSerial.print("\r");
  ////After we send the command we send a carriage return <CR>
  //
  //
  //Serial.print("Got Command: ");
  //Serial.println(cmd);
  //Serial.print("Got hardware type: ");
  //Serial.println(hardwareType);
  //
  ////if (MYHARDWARETYPE == atoi(hardwareType)){
  ////Serial.println("This is my type");
  ////}
  ////else{
  ////Serial.println("This is not my type");
  ////}
  //}


  
  //Serial.println(cmd);
  return 1;
}

int Serial_Processing::strcicmp(char const *a, char const *b)
{
  for (;; a++, b++) {
    int d = tolower(*a) - tolower(*b);
    if (d != 0 || !*a)
    return d;
  }
}


// default destructor
Serial_Processing::~Serial_Processing()
{
} //~Serial_Processing
