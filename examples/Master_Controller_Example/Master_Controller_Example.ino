/*
Arduino_I2C_Port_Expander v0.1.0 - beta

This is a simple blink example. It blinks pin 13 on the slave board.

For detailed instructions. Please visit this project on Github.
https://github.com/jaretburkett/Arduino-I2C-Port-Expander

Upload this code onto an Arduino Compatible microcontroller through
the Arduino IDE. Connect to slave GND, VCC, SCL, SDA. Be sure to use 
pullup resistors on the I2C lines. 

Command examples for library assigned to io. with EXPAND io(0x01);
change to fit your needs. 

Commands:

io.digitalWrite(pin, HIGH | LOW); - writes pin high or low
io.digitalRead(pin); - Returns pin value as integer. 0 for low or 1 for high
io.digitalReadPullup(pin); - Same as digital read, but activates the  internal pullup resistor first. 
io.analogRead(pin); - Returns analog read val as int. Must call slaves digital pin number not A0. 
io.analogWrite(pin, 0-255); - writes pwm to pin. Must be a pwm capable pin. 

This code is released under a MIT license. 
Created by Jaret Burkett
*/

// include wire library first
#include <Wire.h>
// include port expander library second
#include <Arduino_I2C_Port_Expander.h>

/* 	
Initialize the library class and name it "io". You can name it whatever
you want. For multiple port expanders, give each their own unique name. 
*/
EXPAND io(0x01);  		//initialize an instance of the class with address 0x01
// EXPAND io2(0x02); 	// second port expander

void setup()
{
  Wire.begin();
  Serial.begin(57600);  // start serial for output
  while(!Serial){}		// wait for serial port. Only needed on Leonardo based boards
}
 
void loop()
{
io.digitalWrite(13, HIGH);
delay(500);
io.digitalWrite(13, LOW);
delay(500);
}


 


