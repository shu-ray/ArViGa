// All 16 gamepad buttons
const int buttonPins[16] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37};
int lastButtState[16] = {};
const int buttonPrefix = 22;
int buttonState = 0;
// Joysticks
const int joystickLXpin = A0; const int joystickLYpin = A1; const int joystickRXpin = A2; const int joystickRYpin = A3;
// Idle state reads
int joysticksLXidle = 0; int joysticksLYidle = 0; int joysticksRXidle = 0; int joysticksRYidle = 0;
// Reads
int joysticksLXread = 0; int joysticksLYread = 0; int joysticksRXread = 0; int joysticksRYread = 0;
/*
     The status integer used to discriminate operations about sending state of;
        - status = 0x9n or 1001 NNNN : pressed state of any those 16 buttons
        - status = 0x8n or 1000 NNNN : released state of any those 16 butts
        - status = 0xAn or 1010 NNNN: right thumbstick
        - status = 0xBn or 1011 NNNN: left thumbstick
        - n is the index of the gamepad starting from 0 and the max is 4
*/
void sendXinput(byte status, byte buttonByte, int axisX, int axisY){
    word dataBytesX = 0; word dataBytesY = 0;                      // axisX or Y remapped to word type
    byte MSBx = 0; byte MSBy = 0; byte LSBx = 0; byte LSBy = 0;    // significant bytes
    Serial.write(status);
    switch (status >> 4){
    case 0x9: Serial.write(buttonByte); break;
    case 0x8: Serial.write(buttonByte); break;

    // buttonByte is ignored in both cases below
    case 0xA:
      dataBytesX = map(axisX, 0, 1023, 0, 65535); dataBytesY = map(axisY, 0, 1023, 0, 65535);
      // Literally split dataBytes to Most Significant Bit and Least Significant Bit
      MSBx = dataBytesX >> 8; LSBx = dataBytesX << 8;
      MSBy = dataBytesY >> 8; LSBy = dataBytesY << 8;

      // The order of the bytes sent will be important to "feed" the ViGEmClient later by an algorithm
      Serial.write(MSBx); Serial.write(LSBx); Serial.write(MSBy); Serial.write(LSBy); break;
    case 0xB:
      dataBytesX = map(axisX, 0, 1023, 0, 65535); dataBytesY = map(axisY, 0, 1023, 0, 65535);
      MSBx = dataBytesX >> 8; LSBx = dataBytesX << 8;
      MSBy = dataBytesY >> 8; LSBy = dataBytesY << 8;
      Serial.write(MSBx); Serial.write(LSBx); Serial.write(MSBy); Serial.write(LSBy); break;
    }
}

// All 16 gamepad buttons detection and send xinput signal respectively
void buttons(){
  for (int i : buttonPins){
    buttonState = digitalRead(i);
    if (buttonState != lastButtState[i-buttonPrefix]){
      if (buttonState == HIGH){
        switch (i){
          case 22: sendXinput(0x90,0x0,0,0); break;
          case 23: sendXinput(0x90,0x1,0,0); break;
          case 24: sendXinput(0x90,0x2,0,0); break;
          case 25: sendXinput(0x90,0x3,0,0); break;
          case 26: sendXinput(0x90,0x4,0,0); break;
          case 27: sendXinput(0x90,0x5,0,0); break;
          case 28: sendXinput(0x90,0x6,0,0); break;
          case 29: sendXinput(0x90,0x7,0,0); break;
          case 30: sendXinput(0x90,0x8,0,0); break;
          case 31: sendXinput(0x90,0x9,0,0); break;
          case 32: sendXinput(0x90,0xA,0,0); break;
          case 33: sendXinput(0x90,0xB,0,0); break;
          case 34: sendXinput(0x90,0xC,0,0); break;
          case 35: sendXinput(0x90,0xD,0,0); break;
          case 36: sendXinput(0x90,0xE,0,0); break;
          case 37: sendXinput(0x90,0xF,0,0); break;
        }
        lastButtState[i] = HIGH;
      }
    if (buttonState == LOW){
        switch (i){
          case 22: sendXinput(0x80,0x0,0,0); break;
          case 23: sendXinput(0x80,0x1,0,0); break;
          case 24: sendXinput(0x80,0x2,0,0); break;
          case 25: sendXinput(0x80,0x3,0,0); break;
          case 26: sendXinput(0x80,0x4,0,0); break;
          case 27: sendXinput(0x80,0x5,0,0); break;
          case 28: sendXinput(0x80,0x6,0,0); break;
          case 29: sendXinput(0x80,0x7,0,0); break;
          case 30: sendXinput(0x80,0x8,0,0); break;
          case 31: sendXinput(0x80,0x9,0,0); break;
          case 32: sendXinput(0x80,0xA,0,0); break;
          case 33: sendXinput(0x80,0xB,0,0); break;
          case 34: sendXinput(0x80,0xC,0,0); break;
          case 35: sendXinput(0x80,0xD,0,0); break;
          case 36: sendXinput(0x80,0xE,0,0); break;
          case 37: sendXinput(0x80,0xF,0,0); break;
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
    // The 2nd byte (buttonByte parameter) will not be used by the function
    sendXinput(0xA0, 0, joysticksRXread, joysticksRYread);
  }
  if (checkActiveAnalog(joysticksLXread, joysticksLXidle) || checkActiveAnalog(joysticksLYread, joysticksLYidle)){
    sendXinput(0xB0, 0, joysticksLXread, joysticksLYread);
  }
}

void setup() {
  // put your setup code here, to run once:
  for (int i : buttonPins){
    pinMode(i, INPUT_PULLUP);

    // Set last state of all buttons as low (unpressed)
    lastButtState[i] = LOW;
  }

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
  buttons();
  joysticcs();
}
