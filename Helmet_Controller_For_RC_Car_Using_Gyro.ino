/*****************************************************************
Code adapted from:
https://github.com/sparkfun/LSM9DS0_Breakout

The LSM9DS0 is a versatile 9DOF sensor. It has a built-in
accelerometer, gyroscope, and magnetometer.

Hardware setup: This library supports communicating with the
LSM9DS0 over either I2C or SPI. If you're using I2C, these are
the only connections that need to be made:
	LSM9DS0 --------- Arduino
	 SCL ---------- SCL (A5 on older 'Duinos')
	 SDA ---------- SDA (A4 on older 'Duinos')
	 VDD ------------- 3.3V
	 GND ------------- GND
(CSG, CSXM, SDOG, and SDOXM should all be pulled high jumpers on 
  the breakout board will do this for you.)
  
Refer to https://learn.sparkfun.com/tutorials/lsm9ds0-hookup-guide?_ga=1.229536922.721461076.1436525049 for more information
	
The LSM9DS0 has a maximum voltage of 3.6V. Make sure you power it
off the 3.3V rail!
SparkFun Logic Level Converter - Bi-Directional used as we are using 5V Arduino Uno --> https://www.sparkfun.com/products/12009?_ga=1.20421310.721461076.1436525049

Distributed as-is; no warranty is given.
*****************************************************************/

// The SFE_LSM9DS0 requires both the SPI and Wire libraries.
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include <SFE_LSM9DS0.h>

///////////////////////
// Example I2C Setup //
///////////////////////
// GWI - Current set-up is using I2C - 25/07/15
// SDO_XM and SDO_G are both grounded, so our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
// Create an instance of the LSM9DS0 library called `dof` the
// parameters for this constructor are:
// [SPI or I2C Mode declaration],[gyro I2C address],[xm I2C add.]
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

#define PRINT_CALCULATED

#define PRINT_SPEED 500 // X ms between prints
// Default Print_Speed is 500
const int forwardInput = 13; //Refers to arduino digital pins --> alter to match actual set-up
const int reverseInput = 12;
const int leftInput = 10;
const int rightInput = 11;
const int sensPitchPot = A0;
const int sensRollPot = A1;
const int centPitchPot = A2;
const int centRollPot = A3;
float sensitivityPitch, sensitivityRoll;
float centrePitch, centreRoll;
float sensPitchPotValue, sensRollPotValue, centPitchPotValue, centRollPotValue;
boolean forwardOn, reverseOn, leftOn, rightOn;
float pitch, roll;


void setup()
{
  Serial.begin(115200); // Start serial at 115200 bps --> Make sure to change the baud rate in the arduino serial monitor to this so the values can be read during troubleshooting
  uint16_t status = dof.begin(); //Initialises the library
  Serial.print("LSM9DS0 WHO_AM_I's returned: 0x"); //A check for the board to ensure the libraries are installed
  Serial.println(status, HEX);
  Serial.println("Should be 0x49D4");
  Serial.println();
  pinMode(forwardInput, OUTPUT);
  pinMode(reverseInput, OUTPUT);
  pinMode(leftInput, OUTPUT);
  pinMode(rightInput, OUTPUT);

  ReadAndSetPotValues();
  /*centrePitch -> Determines pitch at rest
  centreRoll -> Determine roll at rest
  sensitivityPitch -> Determines the deadzone for the forward and back
  sensitivityRoll -> Determines the deadzone for the left and right
  */
}

void loop()
{
  ReadAndSetPotValues();
  printAccel(); // Needed to calculate the acceleration x, y and z values
  printOrientation(dof.calcAccel(dof.ax), dof.calcAccel(dof.ay), 
                   dof.calcAccel(dof.az)); //where printOrientation is called
  //Serial.println(); //Blank line
  
  if(roll <= (centreRoll - sensitivityRoll)){ 
  digitalWrite(rightInput, LOW);
  digitalWrite(leftInput, HIGH);
  }
  
  else if (roll >= (centreRoll + sensitivityRoll)){
  digitalWrite(leftInput, LOW);
  digitalWrite(rightInput, HIGH);
  }
  
  else {
  digitalWrite(leftInput, LOW);
  digitalWrite(rightInput, LOW);
  }
  
  if(pitch <= (centrePitch - sensitivityPitch)){
  digitalWrite(reverseInput, LOW); 
  digitalWrite(forwardInput, HIGH);
  }
  
  else if (pitch >= (centrePitch + sensitivityPitch)){
  digitalWrite(forwardInput, LOW);
  digitalWrite(reverseInput, HIGH);
  }
  
  else {
  digitalWrite(forwardInput, LOW);
  digitalWrite(reverseInput, LOW);
  }
  
  delay(PRINT_SPEED);
}

void ReadAndSetPotValues()
{
  sensPitchPotValue = analogRead(sensPitchPot);
  sensRollPotValue = analogRead(sensRollPot);
  centPitchPotValue = analogRead(centPitchPot);
  centRollPotValue = analogRead(centRollPot);

  //Sensitivity values (30 is max value at min pot value 0, 0 is min val at max pot value 1023)
  //30 is the maximum sensitivity value, 0 is the minimum sensitivity value.
  sensitivityPitch = map(sensPitchPotValue, 0, 1023, 30,0);
  sensitivityRoll = map(sensRollPotValue, 0,1023, 40,0);

  //Centre values
  centrePitch = map(centPitchPotValue,0, 1023, 30,-30);
  centreRoll = map(centRollPotValue,0, 1023, 20, 0);
}

void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  dof.readAccel();
}

//printOrientation calculates the pitch and roll values used in this program
void printOrientation(float x, float y, float z)
{
  pitch = atan2(x, sqrt(y * y) + (z * z));
  roll = atan2(y, sqrt(x * x) + (z * z));
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;
  
  /*Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 1);
  Serial.print(", ");
  Serial.println(roll, 1);
  */
}
