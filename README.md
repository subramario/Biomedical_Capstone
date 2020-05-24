# Cervical Protection Device
Recipient of the Social Impact Award at the University of Waterloo, **NeXafe** is a cervical protection device designed to protect the elderly from fall associated whiplash damage. This repository contains the software we used to build our award winning 4th Year Mechanical Engineering Capstone!

## Capstone Video Presentation
Watch our capstone showcase video by clicking the link below!
[![](http://img.youtube.com/vi/IGJ12Pg0zsY/0.jpg)](http://www.youtube.com/watch?v=IGJ12Pg0zsY "NeXafe Capstone Video")

## Team Website
https://alijgam.wixsite.com/geriatricfydp

## Code Summary
| Filename                      | Description                                                         | Use Case                         |
| ----------------------------- | ------------------------------------------------------------------- | -------------------------------- |
| `CalibrateCheck.ino` | Reads accelerometer data and outputs bias and scaling parameters. | Arduino sketch. Parameters are used in the `T32FallDetectUntethered.ino` file to ensure accelerometer readings are correctly biased / scaled. 
| `T32FallDetectUntethered.ino` | Final / Production code, run on the Teensy 3.2 microcontroller to execute fall detection and handle input / output peripherals. | Arduino sketch. Runs on the NeXafe device.
| `DataLoggerSPI.py` | Reads accelerometer data from the USB-connected microcontroller + IMU via serial communication. | Python script. Used to log fall profile accelerometer and gyrometer data during physical testing. Data was analyzed to determine NeXafe actuation thresholds.

**Note:** The `MPU9250-master` and `CircularBuffer-master` zip files are Arduino libraries used by the fall detection program. They are required to implement the MPU9250 IMU and circular buffer data structure, respectively.
