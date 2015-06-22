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

// for touchscreen
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#include "ts.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
uint16_t xpos = 0;
uint16_t ypos= 0;
uint16_t zpos = 0;

uint16_t receivedPacket[10]; 
#include <Wire.h>

byte bytesSent = 0;
byte buffer[2];

bool useTS = false;

uint16_t returninfo;

 
void setup(){
	Wire.begin(SlaveDeviceId);      // join i2c bus with Slave ID
	Wire.onReceive(receiveDataPacket); // register talk event
	Wire.onRequest(slavesRespond);  // register callback event
}

void loop(){  
	if(useTS == true){
    	TSPoint p = ts.getPoint();
     	xpos = p.x;
     	ypos = p.y;
     	zpos = p.z;
  	}
}

void receiveDataPacket(int howMany){
	// get bytes from i2c
	bytesSent = 0; // clear byte counter
	for(byte i=0; i < howMany; i++){
		receivedPacket[i] = Wire.read();
	} 
/*
  receivedPacket[0] = method
*/
}
 
void slavesRespond(){
	if(bytesSent == 0){
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
		case 20: // turn on touchscreen
	      returninfo = 1;
	      useTS = true;
	      break;
	    case 21: // Touchscreen Read x
	      returninfo = xpos;
	      break;
	    case 22: // Touchscreen Read y
	      returninfo = ypos;
	      break;
	    case 23: // Touchscreen Read z
	      returninfo = zpos;
	      break;
	    default:
	      returninfo = 99999;
		}
	}
	if(bytesSent == 0){ //send first byte
		buffer[0] = returninfo >> 8;
		buffer[1] = returninfo & 0xff;
		Wire.write(buffer[0]);
		bytesSent++;
	} else if(bytesSent == 1){ // send second byte
		Wire.write(buffer[1]);
		bytesSent = 0; // clear byte counter
	}
}
 
