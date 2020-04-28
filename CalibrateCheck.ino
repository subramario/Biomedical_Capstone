/*
CalibrateCheck.ino
Ouput biases+scale factors for accelerometer & magnetometer
*/
/*
Two ways to calibrate:
  1) Set it down so x and y = 0, z = -9.8 --> we used this one
  2) Use specific motion (Figure 8 motion) to gather values

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
  
  Serial.println("Done");
}

void loop() {}
