# DualShockDrive
Arduino project to allow DualSence 4 contoller apply outputs for servos and brushless motors

This was designed to work as a controller for an RC controller for a hovercraft like this [project](https://howtomechatronics.com/projects/diy-arduino-based-rc-hovercraft/).

This Arduino project allows the DS4 to control steering via a servo, lift and thrust via brushless motors.

- Left hat control a servo on pin 3 that would control a servo for steering.
- R2 controls a brushless motor on pin 4 for thrust
- L2 controls a brushless motor on pin 5 for lift


- If thrust is at 75% or more the controller will rumble.
- If thrust is at 90% or more the controller will rumble on high.

