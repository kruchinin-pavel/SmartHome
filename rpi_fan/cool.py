import os
from time import sleep
import signal
import sys
import RPi.GPIO as GPIO


def getCPUtemperature() -> float:
    res = os.popen('sensors').readlines()[2]
    temp = float(res.split(':')[-1].replace("°C", ""))
    return temp


def setup(pin):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(pin, GPIO.OUT)
    GPIO.setwarnings(False)
    GPIO.output(18, 0)

try:
    GPIO.cleanup()  # resets all GPIO ports used by this program
    setup(18)
    while True:
        cp_utemperature = getCPUtemperature()
        if cp_utemperature > 80:
            print(f'Cooling from  {cp_utemperature}')
            GPIO.output(18, 1)
        elif cp_utemperature < 50:
            GPIO.output(18, 0)
            print(f'Cool off: {cp_utemperature}')
        else:
            print(f'Nothing cause {cp_utemperature}')
        sleep(10)
finally:
    GPIO.cleanup()  # resets all GPIO ports used by this program
