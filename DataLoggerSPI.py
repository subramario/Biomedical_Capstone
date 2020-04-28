# -*- coding: utf-8 -*-
##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial

serial_port = 'COM6'; #based on what IDE says once you connect laptop to Arduino
baud_rate = 230400; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "250Hz_StumblingBack_05.csv";

ser = serial.Serial(serial_port, baud_rate)
with open(write_to_file_path, "w+") as output_file:
    output_file.write("Time,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ,Temp,\n")
    while True:
        line = ser.readline();
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        print(line);
        output_file.write(line);
