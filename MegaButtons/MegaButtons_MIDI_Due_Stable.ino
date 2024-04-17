#include <MIDIUSB.h>
 
// Количество кнопок
const int N_BUTTONS = 16;
// MIDI канал (1-16)
const int MIDI_CHANNEL = 1;

// Задержка для антидребезга в миллисекундах
const unsigned long DEBOUNCE_DELAY = 50;
// Массивы для хранения времени последнего действия кнопки
unsigned long lastDebounceTime[N_BUTTONS];
bool lastButtonState[N_BUTTONS] = {HIGH};

// Пины, к которым подключены кнопки
int buttonPins[N_BUTTONS] = {52, 44, 48, 46, 42, 40, 38, 36, 37, 32, 7, 8, 9, 10, 11, 12};
 
// Структура для хранения данных сдвигового регистра
struct ShiftRegister {
  int clockPin;
  int dataPin;
  int latchPin;
  bool ledStates[N_BUTTONS / 2];
};
 
// Инициализация сдвиговых регистров
ShiftRegister registers[2] = {
  {49, 53, 51, {0}},
  {27, 23, 25, {0}}
};
 
// Функция отправки данных в сдвиговый регистр
void updateShiftRegister(ShiftRegister &reg) {
  digitalWrite(reg.latchPin, LOW);
  for (int i = N_BUTTONS / 2 - 1; i >= 0; i--) {
    digitalWrite(reg.clockPin, LOW);
    digitalWrite(reg.dataPin, reg.ledStates[i]);
    digitalWrite(reg.clockPin, HIGH);
  }
  digitalWrite(reg.latchPin, HIGH);
}
 
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel == MIDI_CHANNEL - 1) {
    for (int i = 0; i < N_BUTTONS; i++) {
      if (pitch == i) {
        int regIndex = i / (N_BUTTONS / 2);
        int ledIndex = i % (N_BUTTONS / 2);
        registers[regIndex].ledStates[ledIndex] = true;
        updateShiftRegister(registers[regIndex]);
        break;
      }
    }
  }
}
 
void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel == MIDI_CHANNEL - 1) {
    for (int i = 0; i < N_BUTTONS; i++) {
      if (pitch == i) {
        int regIndex = i / (N_BUTTONS / 2);
        int ledIndex = i % (N_BUTTONS / 2);
        registers[regIndex].ledStates[ledIndex] = false;
        updateShiftRegister(registers[regIndex]);
        break;
      }
    }
  }
}
 
void setup() {
  // Настройка кнопок и инициализация lastDebounceTime
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastDebounceTime[i] = 0;
  }
 
  // Настройка сдвиговых регистров
  for (int i = 0; i < 2; i++) {
    pinMode(registers[i].clockPin, OUTPUT);
    pinMode(registers[i].dataPin, OUTPUT);
    pinMode(registers[i].latchPin, OUTPUT);
  }
  
  // Изначально все светодиоды выключены
  for (int i = 0; i < 2; i++) {
    updateShiftRegister(registers[i]);
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
 
  // Обработка состояния кнопок и отправка MIDI сигналов
  for (int i = 0; i < N_BUTTONS; i++) {
    bool currentButtonState = !digitalRead(buttonPins[i]); // состояние кнопки: LOW, если нажата, и HIGH, если не нажата
    unsigned long currentTime = millis();
    // Проверяем, требуется ли обновление состояния в зависимости от времени антидребезга
    if (currentButtonState != lastButtonState[i] && (currentTime - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      lastDebounceTime[i] = currentTime;
      lastButtonState[i] = currentButtonState;

      if (!currentButtonState) { // Если кнопка нажата (состояние LOW)
        // MIDI Note On
        midiEventPacket_t noteOn = {0x09, 0x90 | (MIDI_CHANNEL - 1), static_cast<byte>(i), 127};
        MidiUSB.sendMIDI(noteOn);
      } else { // Если кнопка отпущена (состояние HIGH)
        // MIDI Note Off
        midiEventPacket_t noteOff = {0x08, 0x80 | (MIDI_CHANNEL - 1), static_cast<byte>(i), 0};
        MidiUSB.sendMIDI(noteOff);
      }
      MidiUSB.flush();
    }
  }
}