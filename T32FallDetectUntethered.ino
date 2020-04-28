/*
Advanced_I2C.ino
Brian R Taylor
brian.taylor@bolderflight.com
Github: https://github.com/bolderflight/MPU9250

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//LIBRARIES
#include "MPU9250.h" //Used for the accelerometer
#include <CircularBuffer.h> //Bounded queue data strcuture used to buffer data streams


//GLOBAL VARIABLES
MPU9250 IMU(SPI,10); // Declare MPU9250 object with the MPU-9250 sensor on SPI bus 0 and chip select pin 10
int status;

// Declare buffer / queue for sampling and averaging accelerometer data. Specify buffer size as required
const uint8_t bufferLength = 100; //Only require 100 samples at any time to accurately detect spikes in linear acceleration and angular velocity
CircularBuffer<float, bufferLength> buffer_X_accel; //Buffer to continuously track linear acceleration through accelerometer
CircularBuffer<float, bufferLength> buffer_Y_gyro; //Buffer to continuously track angular velocity through gyrometer

// Pushbutton pin
const int buttonPin = 2; //start button on the jacket --> starts the program

// Indicator pins
const int indicatorRedPin = 3;
const int indicatorGreenPin = 4;
const int actuatorPin = 5; // Actuator pin (connected to solenoid)

// Time counters for measurement duration (in microseconds)
unsigned long measureDuration = 20000000; // 20 seconds
unsigned long startTime;
unsigned long currentTime;
unsigned long deltaTime;
unsigned long blinkDuration = 200000;     // 200 miliseconds
unsigned long blinkTimePrior;


// Sensor flag for control logic
bool sensor_flag = false;
bool trigger_flag = false;

//SETUP LOOP 
void setup() {

  // serial to display data
  /*
  Serial.begin(230400);
  while(!Serial) {}
  */

  // Initialize button pin, actuator LED and indicator LED
  // Format: pinMode(LED_BUILTIN,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(indicatorRedPin,OUTPUT);
  pinMode(indicatorGreenPin,OUTPUT);
  pinMode(actuatorPin,OUTPUT);

  
  status = IMU.begin(); // start communication with IMU 
  
  //Error Checking for failed IMU connection
  if (status < 0) { 

    // Error output for computer connected testing
    /*
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    */

   //Red LED will blink to indicate error
    while(1) { 
      digitalWrite(indicatorRedPin, HIGH);
      delay(250);
      digitalWrite(indicatorRedPin, LOW);
      delay(250);
      }
  }

  // Applying sensitivity ranges--> acquired ranges using 50 trials of controlled fall testing
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G); // setting the accelerometer full scale sensitivity range to +/-8G
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS); // setting the gyroscope full scale sensitivity range to +/-500 deg/s

  /*
  NOTE: Data Output Rate = 1000 / (1+SRD) --> implicit formula within accelerometer libraries
  Data Output Rate and Data Collection/Read rate should be > 2x DLPF (Digital Low Pass Filter) bandwidth
  Implicit noise filtering --> filters out any noise above a certain volume
  */

  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ); // setting DLPF bandwidth to 184 Hz
  IMU.setSrd(1); // setting SRD to 1 for a 500 Hz update rate


  // Apply biases and scale factors to accelerometer --> shifts and scales accelerometer readings based on hardware you are using 
  // Values derived based on flat surface calibration test (CalibrateCheck.ino)
  IMU.setAccelCalX(0.05,1.00);
  IMU.setAccelCalY(0.16,1.00);
  IMU.setAccelCalZ(0.39,0.99);

  // Indicate POWER ON
  digitalWrite(indicatorGreenPin, HIGH);
  blinkTimePrior = micros();
  
}

//MAIN LOOP
void loop() {
  // Variables which store the current reading at any point in time
  float current_X_accel;
  float current_Y_gyro;

  // Initializing variables to store average values of both accelerometer and gyrometer buffers
  float threshold_X_accel = 0;
  float threshold_Y_gyro = 0;

  // Limit markers which govern whether a fall is occuring --> values determined through experimental testing
  float threshold_limit_X_accel = -4;
  float threshold_limit_Y_gyro = -1;
  
  // Set flag and timer on button press
  if(digitalRead(buttonPin) == HIGH){
    sensor_flag = true;
    startTime = micros(); // Returns the number of microseconds since the Arduino board began running the current program

    // Indicate LEDs accordingly
    digitalWrite(indicatorRedPin, LOW);
    digitalWrite(actuatorPin, LOW);
  }

  // Start timer once sensor flag is set --> Basically: Am I going to make measurements during this loop? 
  if(sensor_flag == true){
    currentTime = micros(); // Start timer
    deltaTime = currentTime - startTime; //Time between when we push button and the start of this instance of the loop

    // Runs if deltatime is less than 20 seconds    
    if (deltaTime < measureDuration){

      //digitalWrite(LED_BUILTIN,HIGH);
      IMU.readSensor(); //Pulls measurements from IMU
      current_X_accel = IMU.getAccelX_mss();
      current_Y_gyro = IMU.getGyroY_rads();

      // Blink the green indicator --> shows that IMU is recording data and system is ready for fall detection
      if(currentTime - blinkTimePrior >= blinkDuration){
        if (digitalRead(indicatorGreenPin) == HIGH){
          digitalWrite(indicatorGreenPin, LOW);
        }
        else{
          digitalWrite(indicatorGreenPin, HIGH);
        }
        blinkTimePrior = currentTime;
      }
      
      // Initialization to completely fill the buffer (x-accelerations and y-angular velocity)
      if(!buffer_X_accel.isFull() && !buffer_Y_gyro.isFull()){
        buffer_X_accel.push(current_X_accel);
        buffer_Y_gyro.push(current_Y_gyro);
      }
      
      else{

        // Push next sensor reading
        buffer_X_accel.push(current_X_accel);
        buffer_Y_gyro.push(current_Y_gyro);

        // Used for validating values through computer connected testing
        /*
        Serial.print(deltaTime/1000);
        Serial.print(",");
        Serial.print(current_X_accel);
        Serial.print(",");
        Serial.print(current_Y_gyro);
        Serial.print(",");
        Serial.print('\n');
        */
        
        // Take average of entire buffer to determine current threshold value
        using index_t = decltype(buffer_X_accel)::index_t;
        for (index_t i = 0; i < bufferLength; i++){
          threshold_X_accel += buffer_X_accel[i] / bufferLength;
          threshold_Y_gyro += buffer_Y_gyro[i] / bufferLength;
        }
        
        /*
        1) Current implementation: output message, flush buffer, set flag off to end measurements, turn on LED and solenoid
        2) If current threshold values exceed preset limit thresholds, fall is occuring --> trigger protective measures
        3) Falls have both of these occuring --> use two threshold markers to prevent false triggering due to sporadic accel. readings
        */
        if (threshold_X_accel > threshold_limit_X_accel && threshold_Y_gyro < threshold_limit_Y_gyro){  

          // Used for computer connected testing
          /*
          Serial.print("Fall Detected! ");
          Serial.print('\n');
          Serial.print(deltaTime/1000);
          Serial.print(",");
          Serial.print(threshold_X_accel);
          Serial.print(",");
          Serial.print(threshold_Y_gyro);
          Serial.print(",");
          Serial.print('\n');
          */

          // Fall is detected --> empty buffers (won't reset unless you turn it off/on) and set flags to false
          buffer_X_accel.clear();
          buffer_Y_gyro.clear();
          sensor_flag = false;

          // Turn on actuator and detection LED
          digitalWrite(actuatorPin, HIGH); // Releases CO2 to airbag
          digitalWrite(indicatorRedPin, HIGH); // Indicates that fall is occuring
          digitalWrite(indicatorGreenPin, LOW); // Turn green light off
          delay(3000); // Actuate CO2 flow for 3 seconds to fill airbag to optimal amount
          trigger_flag = true;
        }
      } 

    }
    else{

      // Set flag off if timer exceeded and no fall happens
      sensor_flag = false;
      digitalWrite(indicatorGreenPin, HIGH);
    }
  }
  else{

    // Turn actuator off and reset lights
    if (trigger_flag){
      digitalWrite(actuatorPin, LOW);
      digitalWrite(indicatorRedPin, LOW);
      digitalWrite(indicatorGreenPin, HIGH);
      trigger_flag = false;
    }
    delay(10);
  }
  
}
