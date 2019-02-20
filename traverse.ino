#include <PWM.h>

const int drivePin1 = 9;//LED 1 drive
const int knobPin1 = 2; //LED 1 control
const int opticalswitchinput = 8;
const int directionOutputPin = 13;

int knobValue1 = 0;
int32_t frequency = 30000;
int32_t maxFrequency = 10666;
bool switchstate = false;
bool directionState = false;
int directionTimerLimit = 20;
int timerTime = 0;
int scandelay = 20;
void setup() {
  // put your setup code here, to run once:
  InitTimersSafe();
  SetPinFrequencySafe(9,frequency);
  pinMode(drivePin1, OUTPUT);
  pinMode(directionOutputPin, OUTPUT);
  pinMode(opticalswitchinput, INPUT);
  pwmWrite(drivePin1, 128);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

directionTimerLimit = (maxFrequency / frequency) * 10;


  switchstate = digitalRead(opticalswitchinput);
  if (switchstate != directionState)
  {
    timerTime += scandelay;
    if (timerTime >= directionTimerLimit)
    {
      directionState = !directionState;
      timerTime = 0;
    }
  }

  digitalWrite(directionOutputPin, directionState);

  Serial.println("______________");
  Serial.print("Timer Time: ");
  Serial.println(timerTime);
  Serial.println("______________");

  
  Serial.println("______________");
  Serial.print("Direction timer limit: ");
  Serial.println(directionTimerLimit);
  Serial.println("______________");
  
  Serial.println("______________");
  Serial.print("Direction State: ");
  Serial.println(directionState);
  Serial.println("______________");
  
  
  Serial.println("______________");
  Serial.print("Switch State: ");
  Serial.println(switchstate);
  Serial.println("______________");
  
  knobValue1 = analogRead(knobPin1);
  //Serial.println("___________");
  //Serial.print("Pot: ");
  //Serial.println(knobValue1);
  frequency = map(knobValue1, 0, 1023, 52, maxFrequency);
  //Serial.print("Freq: ");
  //Serial.println(frequency);
  //Serial.println("___________");
  bool success = SetPinFrequencySafe(9,frequency);
  if (success){
    //Serial.println("freq set success");
    pwmWrite(drivePin1, 128);
  }
  delay(scandelay);
}
