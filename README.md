# Arduino I2C Port Expander #

This is a library to turn any Arduino compatible microcontroller into the ultimate I2C port expander. 

## Why is this needed? ##

There are a lot of port expanders out there. I always have the same problem with them. **They only do one thing**. One can read or write digital inputs. Another can write pwm. Still anothher can read analog inputs. They do their own thing well but they can only do that one thing. Most microcontrollers have multiple capabilities, including the microcontroller on Arduino. It has all of these functions and much much more. This makes it the perfect cantidate for the ultimate port expander for any project.

## Getting Started ##

With the Arduino IDE closed, download or clone this repository and place it in the libraries folder. Open up Arduino and go to File > Examples > Arduino I2C Port Expander > Slave I2C Port Expander. Upload this code to your microcontroller that you want to become a port expander. 

Next, load the Master Controller Example from the same examples folder. Load this sketch on the board that you want to be the master. 

Now, connect both the master and the slave together. They must share a ground. Hook up the I2C lines on both. It is best to use a 4.7k resistor to pull them both to 5v or vcc. Then just power them on, and the blink sketch should begin to run on the master and blink the LED on pin 13 on the slave. 

## Functions ##

When you initialize this library on the master, you have to give it a name. For the code examples, we will assume you named it "io".

    io.digitalWrite(pin, HIGH | LOW);
Writes pin high or low

    io.digitalRead(pin);
Returns pin value as integer. 0 for low or 1 for high

    io.digitalReadPullup(pin);
Same as digital read, but activates the  internal pullup resistor first. 

    io.analogRead(pin);
Returns analog read val as int. Must call slaves digital pin number not its analog pin. So use 14 instead of A0.

    io.analogWrite(pin, 0-255);
Writes pwm to pin. Must be a pwm capable pin. 


