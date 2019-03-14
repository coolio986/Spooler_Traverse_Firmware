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

#define MAX_CMD_LENGTH 60


// default constructor
Serial_Processing::Serial_Processing()
{
} //Serial_Processing


void Serial_Processing::Setup(void)
{
  Serial.begin(SERIAL_BAUD);               //Set the hardware serial port to 9600
  Serial.setTimeout(50);
  
  
  
}


int Serial_Processing::CommandsProcess(sCommand *ptrCmds)
{

  if (Serial.available() > 0)
  {
    


    computer_bytes_received = Serial.readBytesUntil(13, computerdata, numberOfBufferBytes); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
    computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
    
    

  }

  if (computer_bytes_received != 0) {             //If computer_bytes_received does not equal zero
    
    CommandParse(ptrCmds, computerdata);
    
    computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0
  }

  return 0;
}


unsigned int Serial_Processing::CommandParse(sCommand *ptrCmds, char str[MAX_CMD_LENGTH])
{

  sCommand cmd_list;

  unsigned int i;
  i=0;
  memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));

  

  

  while(cmd_list.function!=0)
  {
    
    if (strcicmp(str,cmd_list.name)==0){
      return (*cmd_list.function)(0,str);
    }

    i=i+1;
    memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));
  }

  //channel = strtok(computerdata, ";");          //Let's parse the string at each colon
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
  //Serial.println(channel);
  //
  ////if (MYHARDWARETYPE == atoi(channel)){
  ////Serial.println("This is my type");
  ////}
  ////else{
  ////Serial.println("This is not my type");
  ////}
  //}



  Serial.println(str);
  return -1;
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
