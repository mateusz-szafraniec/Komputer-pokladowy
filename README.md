# Car trip computer
Small but "powerful" Arduino-based car trip computer, with OLED display.
It counts fuel usage, remaining fuel, remaining range, average speed, distance, internal/extarnal temperature and even time for 1/4 mile drag race :)
Everything fits into place after original clock.
It works on "low-level" signals from injectors, speed sensor, without OBD.

Partially based on concept & code from: https://github.com/xtensa/xarias

## Demo:
https://www.youtube.com/watch?v=Hbh-Mv5nM38

##  MCU:
ATMEGA328p

## Fuses:
L:0xff H:0xd7 E:0xfc Lock:0xff

##  Libraries:
Board: Barebone Atmega328p @ 16MHz (https://github.com/carlosefr/atmega)

ds3231: https://github.com/rodan/ds3231.git commit: c935468

OneWire:  https://github.com/PaulStoffregen/OneWire version 2.3.3

DS18b20:  https://github.com/milesburton/Arduino-Temperature-Control-Library version 3.7.9

LCD/OLED: https://github.com/mateusz-szafraniec/LiquidCrystal_I2C (my fork of https://github.com/marcoschwartz/LiquidCrystal_I2C)
