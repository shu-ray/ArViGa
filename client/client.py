import time
import vgamepad
import serial   # pyserial
import keyboard

# where incoming byte first landed on.The first nibble (the first 4 bits of the byte) indicate the index of gamepad (max simultaneous gamepad connected is 4 on windows).the next nibble indicate the operation whether button or joystick.
def status_switch():
    pass

# if its button operation,this function will check the next byte as the 16 buttons on gamepad
def button_switch():
    pass

# if its thumbstick operation, this will check next two byte as the coordinates of either X or Y axis
def thumbstick_switch():
    pass

ser = serial.Serial("COM3")
print(ser.name)

while True:
    incoming = ser.read()

    if keyboard.is_pressed("ctrl+alt+z"):
        break

ser.close()