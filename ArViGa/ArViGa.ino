// All 16 gamepad buttons
const int buttonPins[16] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37};
int lastButtState[16] = {};
const int buttonPrefix = 22;

// Joysticks
const int joystickLXpin = A0; const int joystickLYpin = A1; const int joystickRXpin = A2; const int joystickRYpin = A3;
// Idle state reads
int joysticksLXidle = 0; int joysticksLYidle = 0; int joysticksRXidle = 0; int joysticksRYidle = 0;
// Reads
int joysticksLXread = 0; int joysticksLYread = 0; int joysticksRXread = 0; int joysticksRYread = 0;
/*
     The status integer used to discriminate operations about sending state of;
        - status = 0x1n : pressed state of any those 16 buttons
        - status = 0x2n : termination of the pressed state (i.e the button had just been released)
        - status = 0x3n : right thumbstick
        - status = 0x4n : left thumbstick
        - n is the index of the gamepad starting from 0 and the max is 4
*/
void sendXinput(byte status, byte button, int axisX, int axisY){
    switch (status >> 4){
    case 0x1: Serial.write(status); Serial.write(button); break;
    case 0x2: Serial.write(status); Serial.write(button); break;
    case 0x3: 
      Serial.write(status);
      int dataBytesX = map(axisX, 0, 1023, 0, 65535);
      int dataBytesY = map(axisY, 0, 1023, 0, 65535);
      byte MSB = dataBytesX >> 8 ;
      byte ASB = MSB >> 4;   // Average significant byte
      byte LSB = dataBytesX << 8;
    case 0x4: 
      Serial.write(status);
    }
}

void buttons(){
  for (int i : buttonPins){
    int buttonState = digitalRead(i);
    if (buttonState != lastButtState[i-buttonPrefix]){
      if (buttonState == HIGH){
        switch (i){
          case 22: sendXinput(0x10,0x0,0,0); break;
          case 23: sendXinput(0x10,0x1,0,0); break;
          case 24: sendXinput(0x10,0x2,0,0); break;
          case 25: sendXinput(0x10,0x3,0,0); break;
          case 26: sendXinput(0x10,0x4,0,0); break;
          case 27: sendXinput(0x10,0x5,0,0); break;
          case 28: sendXinput(0x10,0x6,0,0); break;
          case 29: sendXinput(0x10,0x7,0,0); break;
          case 30: sendXinput(0x10,0x8,0,0); break;
          case 31: sendXinput(0x10,0x9,0,0); break;
          case 32: sendXinput(0x10,0xA,0,0); break;
          case 33: sendXinput(0x10,0xB,0,0); break;
          case 34: sendXinput(0x10,0xC,0,0); break;
          case 35: sendXinput(0x10,0xD,0,0); break;
          case 36: sendXinput(0x10,0xE,0,0); break;
          case 37: sendXinput(0x10,0xF,0,0); break;
        }
        lastButtState[i] = HIGH;
      }
    if (buttonState == LOW){
        switch (i){
          case 22: sendXinput(0x20,0x0,0,0); break;
          case 23: sendXinput(0x20,0x1,0,0); break;
          case 24: sendXinput(0x20,0x2,0,0); break;
          case 25: sendXinput(0x20,0x3,0,0); break;
          case 26: sendXinput(0x20,0x4,0,0); break;
          case 27: sendXinput(0x20,0x5,0,0); break;
          case 28: sendXinput(0x20,0x6,0,0); break;
          case 29: sendXinput(0x20,0x7,0,0); break;
          case 30: sendXinput(0x20,0x8,0,0); break;
          case 31: sendXinput(0x20,0x9,0,0); break;
          case 32: sendXinput(0x20,0xA,0,0); break;
          case 33: sendXinput(0x20,0xB,0,0); break;
          case 34: sendXinput(0x20,0xC,0,0); break;
          case 35: sendXinput(0x20,0xD,0,0); break;
          case 36: sendXinput(0x20,0xE,0,0); break;
          case 37: sendXinput(0x20,0xF,0,0); break;
        }
        lastButtState[i] = LOW;
      }
    }
  }
}

bool checkActiveAnalog(int readVal, int idleVal){
  if (readVal > (idleVal + 1) && readVal < (idleVal - 1)){
    return true;
  }
  else return false;
}

void joysticcs(){
  joysticksRXread = analogRead(joystickRXpin); joysticksRYread = analogRead(joystickRYpin);
  joysticksLXread = analogRead(joystickLXpin); joysticksLYread = analogRead(joystickLYpin);
  if (checkActiveAnalog(joysticksRXread, joysticksRXidle) || checkActiveAnalog(joysticksRYread, joysticksRYidle)){
    // Byte of 0x14 to 0x19 dont't have any meaning since the max gamepad index is 0x13
    sendXinput(0x40, 0x14, )
  }
  if (checkActiveAnalog(joysticksLXread, joysticksLXidle) || checkActiveAnalog(joysticksLYread, joysticksLYidle)){}
}

void setup() {
  // put your setup code here, to run once:
  for (int i : buttonPins){
    pinMode(i, INPUT_PULLUP);

    // Set last state of all buttons as low (unpressed)
    lastButtState[i] = LOW;
  }

  int i = 0;
  while (millis() < 2000){
    if (millis() % 200 == 0){
      joysticksLXidle += analogRead(joystickLXpin); joysticksLYidle += analogRead(joystickLYpin);
      joysticksRXidle += analogRead(joystickRXpin); joysticksRYidle += analogRead(joystickRYpin);
    }
  }
  joysticksLXidle = round(joysticksLXidle / 10); joysticksLYidle = round(joysticksLYidle / 10);
  joysticksRXidle = round(joysticksRXidle / 10); joysticksRYidle = round(joysticksRYidle / 10);

}

void loop() {
  // put your main code here, to run repeatedly:

}