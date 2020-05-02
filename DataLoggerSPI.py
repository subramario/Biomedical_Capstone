# -*- coding: utf-8 -*-
##############
# Script listens to an open Arduino serial port and writes contents into a file
# Requires pySerial to be installed 
# HOW TO USE:
# 1) Connect the Arduino to a computer and note its Port and Baud Rate
# 2) Press the button on the Arduino to begin serial communication (Arduino will send data for a specified duration)
# 3) Check if the Arduino serial is receving data. If so, this script can be run
# 4) Run the DataLoggerSPI.py script to begin writing data to file
# 5) Data will be recorded to file until the Arduino (and serial) times out

import serial

serial_port = 'COM6' # Based on what IDE says once you connect laptop to Arduino
baud_rate = 230400   # In arduino, Serial.begin(baud_rate)
time_out = 5          # Serial will time out after 5 seconds without receiving data
write_to_file_path = "250Hz_Motion_01.csv"

# Open file to store data and write header row
output_file = open(write_to_file_path, "w+")
output_file.write("Time,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ,Temp,\n")

# Initialize serial connection to Arduino (until timeout occurs)
with serial.Serial(serial_port, baud_rate, timeout = time_out) as ser:
    
    # Read and output serial data line-by-line
    line = ser.readline()
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line)
    output_file.write(line)

# Close file
output_file.close()
