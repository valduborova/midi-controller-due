#include <MIDIUSB.h>

int buttonState[8]; // массив состояний кнопок по напряжению
int buttonFlag[8]; // массив состояний кнопок (1 - включено/ 0 - выключено)
int button[8] = {33, 24, 15, 5, 6, 11, 10, 9}; // пины кнопок
int btnEnc[8] = {53, 69, 68, 67, 66, A11, A10, A9}; // пины кнопок энкодера
int btneFlag[8];
int btneState[8]; // Массив состояний кнопок энкодера
int encA[8] = {45, 41, 40, 38, 36, 44, 46, 50}; // Пины выхода A энкодера
int encB[8] = {43, 42, 39, 37, 35, 47, 49, 51}; // Пины выхода B энкодера
int encAlast[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int encState[8];

unsigned long lastDebounceTime[8]; // массив последнего времени срабатывания кнопки
unsigned long debounceDelay = 50; // задержка дебаунса в миллисекундах

// Структура для хранения данных сдвигового регистра
struct ShiftRegister {
  int clockPin;
  int dataPin;
  int latchPin;
  bool ledStates[8];
};

// Инициализация сдвигового регистра
ShiftRegister registerD = {4, 8, 7, {0, 0, 0, 0, 0, 0, 0, 0}};

byte byteToSend = 0; // восьмибитовое число, обозначающие включенные кнопок
void setup() {

  // Serial.begin(9600);
  pinMode(registerD.clockPin, OUTPUT);
  pinMode(registerD.dataPin, OUTPUT);
  pinMode(registerD.latchPin, OUTPUT);
  
  for (int i = 0; i < 8; i++){
    pinMode(button[i], INPUT);
    pinMode(btnEnc[i], INPUT);
    pinMode(encA[i], INPUT);
    pinMode(encB[i], INPUT);
  }

  // Изначально все светодиоды выключены
  updateShiftRegister(registerD);

  for (int i = 0; i < 8; i++) {
    lastDebounceTime[i] = 0;
  }
}

void loop() {
  // Обработка входящих USB MIDI сообщений
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.header == 0x9) {
        handleNoteOn(rx.byte1 & 0xF, rx.byte2, rx.byte3);
      }
      else if (rx.header == 0x8) {
        handleNoteOff(rx.byte1 & 0xF, rx.byte2, rx.byte3);
      }
    }
  } while (rx.header != 0);
  // Обработка кнопок 
  for (int i = 0; i < 8; i++){
    int reading = digitalRead(button[i]); // Текущее чтение состояния кнопки

    // Если состояние кнопки изменилось, сбрасываем таймер дебаунса
    if (reading != buttonState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    // Проверяем, прошло ли достаточно времени для стабилизации состояния кнопки
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // Если состояние изменилось, обновляем его
      if (reading != buttonFlag[i]) {
        buttonFlag[i] = reading;
        // Нажатие или отпускание кнопки
        if (buttonFlag[i] == HIGH){
          bitWrite(byteToSend, i, HIGH);      
          // Serial.println("btn " + String(i) + " 1");
          noteOn(0, i);
        } else {
          bitWrite(byteToSend, i, LOW);      
          // Serial.println("btn " + String(i) + " 0");
          noteOff(0, i);
        }
      }
    }
    // Сохраняем последнее чтение состояния
    buttonState[i] = reading;
  }
  // Обработка энкодера как кнопки
  for (int j = 0; j < 8; j++){
    btneState[j] = digitalRead(btnEnc[j]);
  // Нажатие на кнопку  
    if (btneState[j] == LOW && btneFlag[j] == 0){
      btneFlag[j] = 1;
      // Serial.println("bte " + String(j) + " 1");
      noteOn(0, 8 + j);
    }
  // Отпускание кнопки 
    if (btneState[j] == HIGH && btneFlag[j] == 1){
      btneFlag[j] = 0;
      // Serial.println("bte " + String(j) + " 0");
      noteOff(0, 8 + j);
    }
  }
  // Вращение энкодера
  for (int k = 0; k < 8; k++){
    encState[k] = digitalRead(encA[k]);
    if ((encAlast[k] == LOW) && (encState[k] == HIGH)){
      if (digitalRead(encB[k]) == LOW){
        // Serial.println("enc " + String(k) + " R");
        controlChange(0, k, 0);
      }
      if (digitalRead(encB[k]) == HIGH){
        // Serial.println("enc " + String(k) + " L");
        controlChange(0, k, 127);
      }
    }
    encAlast[k] = encState[k];
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void noteOn(byte channel, byte control) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, static_cast<byte>(control), 127};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte control) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, static_cast<byte>(control), 0};
  MidiUSB.sendMIDI(noteOff);
}

// Функция отправки данных в сдвиговый регистр
void updateShiftRegister(ShiftRegister &reg) {
  digitalWrite(reg.latchPin, LOW);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(reg.clockPin, LOW);
    digitalWrite(reg.dataPin, reg.ledStates[i]);
    digitalWrite(reg.clockPin, HIGH);
  }
  digitalWrite(reg.latchPin, HIGH);
}
 
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  for (int i = 0; i < 8; i++) {
    if (pitch == i) {
      registerD.ledStates[i] = true;
      updateShiftRegister(registerD);
      break;
    }
  }
}

 
void handleNoteOff(byte channel, byte pitch, byte velocity) {
  for (int i = 0; i < 8; i++) {
    if (pitch == i) {
      registerD.ledStates[i] = false;
      updateShiftRegister(registerD);
      break;
    }
  }
}
