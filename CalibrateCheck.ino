/*
CalibrateCheck.ino
Use Case:
Ouput the biases and scale factors for accelerometer & magnetometer

Calibration can be done in two ways:
  1) [Used in this program] Set the accelerometer on a flat surface and run the script.
      The accelerometer calculates the deltas between current readings and the expected values (x = 0, y = 0, z = -9.8).
      It sends the bias and scale values to the serial for output.
  2) Use specific motion (Figure 8 motion) to gather values.
*/

#include "MPU9250.h"

// an MPU-9250 object on SPI bus 0 with chip select 10
MPU9250 Imu(Wire,0x68);
int status;
float value;

void setup() {
  
  // serial to display instructions
  Serial.begin(115200);
  while(!Serial) {}
  
  // start communication with IMU 
  status = Imu.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  
  // Print out bias values and scale factors
  value = Imu.getAccelBiasX_mss();
  Serial.print("Accel Bias X = ");
  Serial.println(value);
  value = Imu.getAccelScaleFactorX();
  Serial.print("Accel Scale X = ");
  Serial.println(value);
  value = Imu.getAccelBiasY_mss();
  Serial.print("Accel Bias Y = ");
  Serial.println(value);
  value = Imu.getAccelScaleFactorY();
  Serial.print("Accel Scale Y = ");
  Serial.println(value);
  value = Imu.getAccelBiasZ_mss();
  Serial.print("Accel Bias Z = ");
  Serial.println(value);
  value = Imu.getAccelScaleFactorZ();
  Serial.print("Accel Scale Z = ");
  Serial.println(value);
  value = Imu.getMagBiasX_uT();
  Serial.print("Mag Bias X = ");
  Serial.println(value);
  value = Imu.getMagScaleFactorX();
  Serial.print("Mag Scale X = ");
  Serial.println(value);
  value = Imu.getMagBiasY_uT();
  Serial.print("Mag Bias Y = ");
  Serial.println(value);
  value = Imu.getMagScaleFactorY();
  value = Imu.getMagBiasZ_uT();
  Serial.print("Mag Scale Y = ");
  Serial.println(value);
  Serial.print("Mag Bias Z = ");
  Serial.println(value);
  value = Imu.getMagScaleFactorZ();
  Serial.print("Mag Scale Z = ");
  Serial.println(value);
  
  Serial.println("Calibration Done");
}

void loop() {}
