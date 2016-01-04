# Drone Intel Edison MCU

This project is part of a Flight Controller for a Quadcopter based on an Intel Edison board.
This targets the MCU of the Intel Edison to provide :
- An UART interface with the Radio Controller through a XBEE ZB-PRO 2.4GHz module with a simple application protocol
- An I2C interface with the 9DOF Sparkfun Block for Intel Edison (https://www.sparkfun.com/products/13033)
- An I2C interface with the PWM Sparkfun Block for Intel Edison (https://www.sparkfun.com/products/13042)
- An UART interface with an application running on the host CPU of the Intel Edison board
