# MIDI Контроллер на Arduino Due

Этот код для платы Arduino Due позволяет создать MIDI контроллер с 8 подсвечиваемыми кнопками и 8 вращающимися энкодерами, которые также могут нажиматься как кнопки.

## Разъемы

Для работы устройства в коде используются следующие пины:

- `registerD[]` - Пины для работы с регистром сдвига (управление подсветкой кнопок).
  - `registerD[0]` - пин Clock.
  - `registerD[1]` - пин Data.
  - `registerD[2]` - пин Latch.
- `button[]` - Пины для подключения кнопок.
- `btnEnc[]` - Пины для подключения кнопок энкодеров.
- `encA[]` - Пины выхода A массива энкодеров.
- `encB[]` - Пины выхода B массива энкодеров.

## Установка

1. Подключите вашу плату Arduino Due к компьютеру.
2. Откройте данный код в Arduino IDE.
3. Назначьте ноги платы, в соответствии со своей схемой подключения, если они отличаются от указанных в коде.
4. Загрузите код на вашу плату Arduino Due.

## Использование

После загрузки кода на плату Arduino Due, плата начнёт работать как MIDI устройство. Кнопки и энкодеры будут отсылать MIDI сигналы в соответствии с их состоянием.

- Нажатие кнопки или кнопки на энкодере отправляет сигнал "note on" для соответствующего контрола.
- Отпускание той же кнопки отправляет сигнал "note off" для соответствующего контрола.
- Вращение энкодера вправо или влево отправляет сигнал "control change" с определённым значением.

## Настройка

Перед использованием убедитесь, что все пины подключены правильно. Вы можете изменить конфигурацию пинов в переменных `registerD[]`, `button[]`, `btnEnc[]`, `encA[]`, и `encB[]` в соответствии с вашей схемой.

## Замечания

- Для работы с MIDI обязательно нужно, чтобы подключенное устройство поддерживало MIDI через USB.
- Отладочные сообщения закомментированы. Для отладки раскомментируйте строки с вызовами `Serial.println()`, и убедитесь, что начальная скорость порта Serial правильная (в данном случае это 9600 бод).