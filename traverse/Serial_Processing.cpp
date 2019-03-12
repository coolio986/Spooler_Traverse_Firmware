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


// default constructor
Serial_Processing::Serial_Processing()
{
} //Serial_Processing


void Serial_Processing::Setup(void)
{
  Serial.begin(SERIAL_BAUD);               //Set the hardware serial port to 9600
  Serial.setTimeout(50);
  
  
  
}

void Serial_Processing::RunSerialDataLoop(void)
{
  if (Serial.available() > 0)
  {
    
    computer_bytes_received = Serial.readBytesUntil(13, computerdata, numberOfBufferBytes); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
    computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
    
  }

  if (computer_bytes_received != 0) {             //If computer_bytes_received does not equal zero
    channel = strtok(computerdata, ";");          //Let's parse the string at each colon
    cmd = strtok(NULL, ";");                      //Let's parse the string at each colon
    //Open_channel();                               //Call the function "open_channel" to open the correct data path

    if (cmd != 0) {                               //If a command has been sent
      //Serial.print(cmd);
      //Serial.print("\r");
      //altSerial.print(cmd);                       //Send the command from the computer to the Atlas Scientific device using the softserial port
      //altSerial.print("\r");                      //After we send the command we send a carriage return <CR>
      Serial.print("Got Command: ");
      Serial.println(cmd);
      Serial.print("Got hardware type: ");
      Serial.println(channel);

      if (MYHARDWARETYPE == atoi(channel)){
        Serial.println("This is my type");
      }
      else{
        Serial.println("This is not my type");
      }
    }
    computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0


  }

  




}


// default destructor
Serial_Processing::~Serial_Processing()
{
} //~Serial_Processing
