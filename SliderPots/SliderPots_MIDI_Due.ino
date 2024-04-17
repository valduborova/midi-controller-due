#include <MIDIUSB.h>

const int Max = 8;
char Fader[Max] = {A7, A6, A5, A4, A3, A2, A0, A1};
char pin[Max] = {28, 30, 32, 34, 36, 38, 40, 42};
int FAD[Max];
int dFAD[Max];

void setup() {
  for (int i = 0; i < Max; i++){
    pinMode(pin[i],OUTPUT);
    FAD[i] = 0;
  }

  for (int i = 0; i < Max; i++){
    dFAD[i] = analogRead(Fader[i]);
    dFAD[i] = map(dFAD[i], 0, 1023, 0, 255);
    dFAD[i] = constrain(dFAD[i], 0, 255);
    analogWrite(pin[i], dFAD[i]);
    FAD[i] = dFAD[i];
  } 
}

void loop() {
  for (int i = 0; i < Max; i++){
    dFAD[i] = analogRead(Fader[i]);
    dFAD[i] = map(dFAD[i], 0, 1023, 0, 127);
    dFAD[i] = constrain(dFAD[i], 0, 127);
    digitalWrite(pin[i], dFAD[i]);
    if (FAD[i]!= dFAD[i]) {
      controlChange(0, i, dFAD[i]);
    }
    FAD[i] = dFAD[i];
  }
  MidiUSB.read();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
