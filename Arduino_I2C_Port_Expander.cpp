#include "Arduino_I2C_Port_Expander.h" //include the declaration for this class

byte io_DataPacket[3];
byte io_pin;
byte io_method;
byte io_value;
  
//<<constructor>>
EXPAND::EXPAND(uint8_t addr){
  _addr = addr;
  Wire.begin();
}

//<<destructor>>
EXPAND::~EXPAND(){/*nothing to destruct*/}

void EXPAND::digitalWrite(byte pin,byte val){
  io_DataPacket[0] = 1;    // method
  io_DataPacket[1] = pin;  // pin
  io_DataPacket[2] = val;  // val
  sendDataPacket();
  int received = receiveResponse();
}

int EXPAND::digitalRead(byte pin){
  io_DataPacket[0] = 2;    // method
  io_DataPacket[1] = pin;  // pin
  sendDataPacket();
  int received = receiveResponse();
  return received;
}

int EXPAND::digitalReadPullup(byte pin){
  io_DataPacket[0] = 3;    // method
  io_DataPacket[1] = pin;  // pin
  sendDataPacket();
  int received = receiveResponse();
  return received;
}

void EXPAND::analogWrite(byte pin,byte val){
  io_DataPacket[0] = 4;    // method
  io_DataPacket[1] = pin;  // pin
  io_DataPacket[2] = val;  // val
  sendDataPacket();
  int received = receiveResponse();
}

int EXPAND::analogRead(byte pin){
  io_DataPacket[0] = 5;    // method
  io_DataPacket[1] = pin;  // pin
  sendDataPacket();
  int received = receiveResponse();
  return received;
}

void EXPAND::sendDataPacket(){
  Wire.beginTransmission(_addr);
  Wire.write(io_DataPacket, 3);
  Wire.endTransmission(true);
  delay(1);
}

int EXPAND::receiveResponse(){
  unsigned int receivedValue;
  unsigned long start = millis();
  int available = Wire.requestFrom(_addr, (byte)2);
  //Wire.endTransmission(true);
  if(available == 2)
  {
    receivedValue = Wire.read() << 8 | Wire.read(); // combine two bytes into integer
    
  }
  else
  {
    Serial.print("ERROR: Unexpected number of bytes received - ");
    Serial.println(available);
  }
  
  return receivedValue;  
}


