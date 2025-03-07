#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // For dongles that have hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

Servo dirServo;
Servo liftMotor;
Servo thrustMotor;

int leftHatX = 128;
int leftHatY = 128;
int rightHatX = 128;
int rightHatY = 128;
int l2Value = 0;
int r2Value = 0;

int dirServoPin    = 3;
int thrustMotorPin = 4;
int liftMotorPin   = 5;

int servoMinAngle = 0;
int servoMaxAngle = 180;
int servoAngle = 90;

int motorMaxValue = 2200;
int motorMinValue = 800;
int thrust = 1500;
int lift = 1500;

bool rumbleLow = false;
bool rumbleHigh = false;
bool start = true;

void setup() {
  dirServo.attach(dirServoPin);
  liftMotor.attach(liftMotorPin);
  thrustMotor.attach(thrustMotorPin);

  Serial.begin(9600);
  Serial.print(F("\r\nON"));
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}

void loop() {
  Usb.Task();
  
  // turn off motors at the start so 
  // they don't go to full power when the arduino boots
  if (start) {
    start = false;
    liftMotor.write(motorMinValue);
    thrustMotor.write(motorMinValue);
  }

  int newServoAngle = servoAngle;
  int newThrust = thrust;
  int newLift = lift;

  if (PS4.connected()) {

    leftHatX = PS4.getAnalogHat(LeftHatX);
    newServoAngle = map(leftHatX, 0, 255, servoMinAngle, servoMaxAngle);

    r2Value = PS4.getAnalogButton(R2);
    newThrust = map(r2Value, 0, 255, motorMinValue, motorMaxValue);

    l2Value = PS4.getAnalogButton(L2);
    newLift = map(l2Value, 0, 255, motorMinValue, motorMaxValue);

    if (newServoAngle != servoAngle) {
      servoAngle = newServoAngle;
      Serial.print("\r\nservoAngle ");
      Serial.print(servoAngle);
      Serial.print("\r\n");
      dirServo.write(servoAngle);
    }

    if (newLift != lift) {
      lift = newLift;
      Serial.print("lift: ");
      Serial.print(lift);
      Serial.print("\r\n");
      liftMotor.write(lift);
    }

    if (newThrust != thrust) {
      thrust = newThrust;
      Serial.print("thrust: ");
      Serial.print(thrust);
      Serial.print("\r\n");
      thrustMotor.write(thrust);
    }

    if (thrust > motorMaxValue * 0.90 && rumbleHigh == false) {
      PS4.setRumbleOn(RumbleHigh);
      rumbleHigh = true;
      rumbleLow = false;
    } else if (thrust > motorMaxValue * 0.75 && rumbleLow == false) {
      PS4.setRumbleOn(RumbleLow);
      rumbleLow = true;
      rumbleHigh = false;
    } else if (thrust < motorMaxValue * 0.75 && (rumbleHigh || rumbleLow)) {
      PS4.setRumbleOff();
      rumbleLow = false;
      rumbleHigh = false;
    }
  }

    // read controller example code
    // leftHatX = PS4.getAnalogHat(LeftHatX);
    // leftHatY = PS4.getAnalogHat(LeftHatY);
    // rightHatX = PS4.getAnalogHat(RightHatX);
    // rightHatY = PS4.getAnalogHat(RightHatY);
    // l2Value = PS4.getAnalogButton(L2);
    // r2Value = PS4.getAnalogButton(R2);
    // PS4.getButtonClick(L1);
    // PS4.getButtonClick(R1);
    // PS4.getButtonClick(L3);
    // PS4.getButtonClick(R3);
    // PS4.getButtonClick(TRIANGLE);
    // PS4.getButtonClick(CIRCLE);
    // PS4.getButtonClick(CROSS);
    // PS4.getButtonClick(SQUARE);
    // PS4.getButtonClick(UP);
    // PS4.getButtonClick(DOWN);
    // PS4.getButtonClick(LEFT);
    // PS4.getButtonClick(RIGHT);
    // PS4.getButtonClick(SHARE);
    // PS4.getButtonClick(OPTIONS);
    // PS4.getButtonClick(TOUCHPAD);
    // PS4.getButtonClick(PS);
    // pitch = PS4.getAngle(Pitch);
    // roll = PS4.getAngle(Roll);
    // PS4.isTouching(0); // is one finger touching 
    // PS4.isTouching(1)); // are two fingers touching
    // xTouch1Loc = PS4.getX(0); // finger 1 x location
    // yTouch1Loc = PS4.getY(0); // finger 1 y location
    // xTouch2Loc = PS4.getX(1); // finger 2 x location
    // yTouch2Loc = PS4.getY(1); // finger 2 y location

    // Control controller example code
    // PS4.setLed(color);  // color Green/Rewd/Blue/Yellow/White ....
    // PS4.setRumbleOn(RumbleLow);
    // PS4.setRumbleOn(RumbleHigh);
    // PS4.setRumbleOff();
    // PS4.setLedFlash(10, 10); // Set LED to blink rapidly
    // PS4.setLedFlash(0, 0); // Turn off blinking LED
}
