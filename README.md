# Komputer pokładowy
Arduino-based car trip computer

Partially based on concept & code from: https://github.com/xtensa/xarias

##  MCU ATMEGA328p

## Fuses:
L:0xff H:0xd7 E:0xfc Lock:0xff

##  Libraries:
Board: Barebone Atmega328p @ 16MHz (https://github.com/carlosefr/atmega)
ds3231: https://github.com/rodan/ds3231.git commit: c935468
OneWire:  https://github.com/PaulStoffregen/OneWire version 2.3.3
DS18b20:  https://github.com/milesburton/Arduino-Temperature-Control-Library version 3.7.9
LCD/OLED: https://github.com/mateusz-szafraniec/LiquidCrystal_I2C (my fork of https://github.com/marcoschwartz/LiquidCrystal_I2C)
