/*
 
For detailed instructions. Please visit this project on Github.
https://github.com/jaretburkett/Arduino-I2C-Port-Expander

Upload this code onto an Arduino Compatible microcontroller through
the Arduino IDE. Connect to master GND, VCC, SCL, SDA. Be sure to use 
pullup resistors on the I2C lines. View the master example to see how to 
communicate to your new I2C port expander. 

Be sure to make the slave id compatible with your master.

ATTiny note:
To use this code with the ATtiny, you will need the TinyWireS library from here.
https://github.com/rambo/TinyWire

This code is released under a MIT license. 
Created by Jaret Burkett
*/

// Make this unique for each device on the same I2C bus
// can be 0x01 - 0xff
const uint8_t SlaveDeviceId = 0x01;

// for touchscreen
#define YP 0  // must be an analog pin
#define XM 1  // must be an analog pin
#define YM 9   // can be a digital pin
#define XP 10   // can be a digital pin

#include "ts.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
uint16_t xpos = 0;
uint16_t ypos= 0;
uint16_t zpos = 0;

#include <TinyWireS.h>          //the ATTiny Wire library
#ifndef TWI_RX_BUFFER_SIZE
	#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

byte bytesSent = 0;
byte buffer[2];

bool useTS = false;

uint16_t returninfo;

// buffer for received command.
uint8_t receivedPacket[10]; 
 
void setup(){
	TinyWireS.begin(SlaveDeviceId);      // join i2c bus with Slave ID
	TinyWireS.onReceive(receiveDataPacket); // register talk event
	TinyWireS.onRequest(slavesRespond);  // register callback event
}
  
void loop(){
	TinyWireS_stop_check();
  // if touchscreen has been activated
  if(useTS == true){
    TSPoint p = ts.getPoint();
     xpos = p.x;
     ypos = p.y;
     zpos = p.z;
  }
}

void receiveDataPacket(byte howMany){
  // get bytes from i2c
  bytesSent = 0; // clear byte counter
	  if (TinyWireS.available()){  
		if (howMany < 1)
		  {   // Sanity-check
			  return;
		  }
		  if (howMany > TWI_RX_BUFFER_SIZE)
		  {   // Also insane number
			  return;
		  }
		// get bytes from i2c
		for(byte i=0; i < howMany; i++){
		  receivedPacket[i] = TinyWireS.receive();
		} 
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
		TinyWireS.send(buffer[0]);// return response to last command
		bytesSent++;
	} else if(bytesSent == 1){ // send second byte
		TinyWireS.send(buffer[1]);// return response to last command
		bytesSent = 0; // clear byte counter
	}
}
 

