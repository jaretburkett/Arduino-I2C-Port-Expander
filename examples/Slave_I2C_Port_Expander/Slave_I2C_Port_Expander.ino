/*
For detailed instructions. Please visit this project on Github.
https://github.com/jaretburkett/Arduino-I2C-Port-Expander

Upload this code onto an Arduino Compatible microcontroller through
the Arduino IDE. Connect to master GND, VCC, SCL, SDA. Be sure to use 
pullup resistors on the I2C lines. View the master example to see how to 
communicate to your new I2C port expander. 

Be sure to make the slave id compatible with your master.
 
This code is released under a MIT license. 
Created by Jaret Burkett
*/

// Make this unique for each device on the same I2C bus
// can be 0x01 - 0xff
const uint8_t SlaveDeviceId = 0x01;

#include <Wire.h>

uint16_t returninfo;

// buffer for received command.
uint8_t receivedPacket[10]; 
 
void setup(){
  Wire.begin(SlaveDeviceId);      // join i2c bus with Slave ID
  Wire.onReceive(receiveDataPacket); // register talk event
  Wire.onRequest(slavesRespond);  // register callback event
}
  
void loop(){
// no need for loop. Requests call internal interrupt.
}

void receiveDataPacket(int howMany){
  // get bytes from i2c
  for(byte i=0; i < howMany; i++){
    receivedPacket[i] = Wire.read();
  } 
/*
  receivedPacket[0] = method
*/
}
 
void slavesRespond(){
  switch(receivedPacket[0]){
    case 1:  // digitalWrite
      /*
        receivedPacket[1] = pin
        receivedPacket[2] = value
      */
      pinMode(receivedPacket[1], OUTPUT); // set pin mode
      digitalWrite(receivedPacket[1], receivedPacket[2]);
      returninfo = 1;
      break;
    case 2:  // digitalRead
      /*
        receivedPacket[1] = pin
      */
      pinMode(receivedPacket[1], INPUT); // set pin mode
      returninfo = digitalRead(receivedPacket[1]);
      break;
    case 3:  // digitalRead pullup
      /*
        receivedPacket[1] = pin
      */
      pinMode(receivedPacket[1], INPUT_PULLUP); // set pin mode
      returninfo = digitalRead(receivedPacket[1]);
      break;
    case 4:  // analogWrite
      /*
        receivedPacket[1] = pin
        receivedPacket[2] = value
      */
      pinMode(receivedPacket[1], OUTPUT); // set pin mode
      analogWrite(receivedPacket[1], receivedPacket[2]);
      returninfo = 1;
      break;
    case 5:   // analogRead
      /*
        receivedPacket[1] = pin
      */
      returninfo = analogRead(receivedPacket[1]);
      break;
  }
  
  uint8_t buffer[2];           // split integer return value into two bytes buffer
  buffer[0] = returninfo >> 8;
  buffer[1] = returninfo & 0xff;
  Wire.write(buffer, 2);       // return response to last command
}
 

