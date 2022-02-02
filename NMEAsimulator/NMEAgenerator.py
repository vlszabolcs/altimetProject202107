import imp
import pynmea2
import serial
import time

ser = serial.Serial('COM4', 9600, timeout=1)  # open serial port

   # write a string


file = open('examples\data.log', encoding='utf-8')
lines=file.readlines()

difstartlow = 0
difstarthigh = 5

first =difstartlow
last = difstarthigh

while True:
    
    onePostion=lines[first:last]
    
    for onePositionLine in onePostion:
       
        serSend=onePositionLine
        print(serSend)
        ser.write(serSend.encode()) 
    
    first=first+6
    last=last+6

    if len(lines) < last :
        first=difstartlow
        last=difstarthigh

    time.sleep(0.5)




print(lines[0:6])
"""""
for line in file.readlines():
    print(line)
"""