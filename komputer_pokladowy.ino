/*
 * Fuses: L:0xff H:0xd7 E:0xfc Lock:0xff
 * Board: Barebone Atmega328p @ 16MHz (https://github.com/carlosefr/atmega)
 */
 
#include <avr/sleep.h>
#include <avr/power.h>
#include <EEPROM.h> //Needed to access the eeprom read write functions

//#define UART_SPEED 115200
#define OLED
#define DEEP_SLEEP
//#define DEBUG
#define COUNT_ABOVE 1 //12 - 8,64km/h; 5 - zliczanie dystansu powyżej 3,6km/h; 1 - 0,72km/h

//#define WELCOME_MESSAGE "   306team.pl"  //
#define VERSION         "1.01"

#define INJ_FLOW_DEFAULT 132 //wydajność wtryskiwacza ml/min

#define SPEED_TICKS_DEFAULT 5000 //ilość impulsów na 1 km

#define MAX_INJ_TICKS 24756 //16384 // czas najdłuższego otwarcia na 1 s  24756 =3/4 s, tj, 23,9 l/h
#define MAX_BARGRAPH_INJ_TICKS 16384  // 1/2 sekundy

#define FULL_TANK_DEFAULT 1023 // odczyt przy pełnym baku
#define EMPTY_TANK_DEFAULT 0 // odczyt przy pustym baku
#define TANK_SIZE_DEFAULT 60 // pojemność baku w litrach
#define FUEL_PRICE_DEFAULT 450 // standardowa cena paliwa 4,5 zł/l
#define RATIO_DEFAULT 2 // półsekwencja 2, monowtrysk 1, 4 - 4 wtryski spięte równolegle lub sekwencja
#define MAX_VOLTAGE_DEFAULT 20000 //4(dzielnik 1/4) * 5V

#define MIN_TEMP -35
#define MAX_TEMP 60
#define ONE_WIRE_BUS 6 // ds18b20 na pinie 6

// menu
#ifdef DEBUG
#define SCREEN_COUNT 12
#else
#define SCREEN_COUNT 10
#endif

#define SCREEN_CLOCK 0
#define SCREEN_MOMENTARY 1
#define SCREEN_ECONOMIZER 2
#define SCREEN_AVERAGE 3
#define SCREEN_TRIP 4
#define SCREEN_COSTS 5
#define SCREEN_FUEL_LEVEL 6
#define SCREEN_ODO 7
#define SCREEN_ACCELERATION 8
#define SCREEN_VOLTAGE 9
#define SCREEN_SETTINGS 10

#define SCREEN_DEBUG1 11
#define SCREEN_DEBUG2 12

#define SETUP_SCREEN_CLOCK 0
#define SETUP_SCREEN_FUEL_PRICE 1
#define SETUP_SCREEN_CLEAR_TRIP 2
#define SETUP_SCREEN_ODO_TO_TRIP 3
#define SETUP_SCREEN_INJ_FLOW 4
#define SETUP_SCREEN_RATIO 5
#define SETUP_SCREEN_SPEED_TICKS 6
#define SETUP_SCREEN_TANK_SIZE 7
#define SETUP_SCREEN_CALIBRATE_VACCU 8
#define SETUP_SCREEN_DOTANKOWANIE 9
#define SETUP_SCREEN_VERSION 10

#define SETUP_SCREEN_COUNT 10

#define REFRESH_RATE 1000 //odświeżanie wyświetlacza co 1 sekundę

// mapa pamięci EEPROM
#define INJ_FLOW_ADDR 0x00
#define SPEED_TICKS_ADDR 0x01 //   0x01..0x02 LSB,MSB
#define TANK_SIZE_ADDR 0x07   //   0x07
#define RATIO_ADDR 0x08      //    0x08
#define passed_seconds_ADDR 0x09       //  0x09..0x0C
#define passed_speed_ticks_ADDR 0x0D  //  0x0D..0x10
#define passed_inj_ticks_ADDR 0x11    //  0x11..0x18
#define ODO_passed_seconds_ADDR 0x19      //  0x19..0x1C
#define ODO_passed_speed_ticks_ADDR 0x1D  //  0x1D..0x20
#define ODO_passed_inj_ticks_ADDR 0x21    //  0x21..0x28
#define FUEL_PRICE_ADDR 0x29              //  0x29..0x2A
#define MSDO100_ADDR 0x2A                 //  0x2A..0x2D
#define MSNACWIARTKE_ADDR 0x2E            //  0x2E..0x31
#define MAX_VOLTAGE_ADDR 0x32             //  0x32..0x33
#define DOTANKOWANIE_ADDR 0x34            //  0x34..0x35
#define OSCCAL_ADDR 0x36                  //  0x36
#define UNLOCK_ADDR 0x37                  //  0x37..0x38
#define TEST_BYTE 0x39                    //  0x39

// klawisze
#define UPSHORT 1
#define DOWNSHORT 2
#define UPLONG 3
#define DOWNLONG 4
#define DEBOUNCE 30

// piny
#define PIN_INJ 4 // wtryskiwacz
#define PIN_SPEED 5 // VSS
#define PIN_INT0 2 //interrupt0 - 32kHz
#define PIN_IGN 3 // stacyjka

#define PIN_PWR 9 // sterowanie zasilaniem wyświetlacza i RTC

#define PIN_UP 7 // przycisk góra
#define PIN_DOWN 8 // przycisk dół

#define PIN_ACCU A0 // pomiar +12V
#define PIN_FUEL_METER A1 // pomiar stanu paliwa

//RTC
#include "ds3231.h" // https://github.com/rodan/ds3231.git  c935468
//#define BUFF_MAX 128

//Temperatura
#include <OneWire.h> // https://github.com/PaulStoffregen/OneWire 2.3.3
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library 3.7.9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // https://github.com/mateusz-szafraniec/LiquidCrystal_I2C 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// zmienne
byte INJ_FLOW;  //przepływ wtryskiwacza w ml/min
byte RATIO;
unsigned int SPEED_TICKS;  //ilość impulsów na 1km
unsigned int FUEL_PRICE;  //cena paliwa w groszach
unsigned int MAX_VOLTAGE; //maksymalne napięcie akumulatora, konieczne do kalibracji [mV]
unsigned long msDo100;
unsigned long msNaCwiartke;

byte TANK_SIZE;  //pojemność baku [L]
unsigned int DOTANKOWANIE; // dodatkowe zatankowane paliwo [L]
unsigned int UNLOCK_CODE;

// keep the state of register ADCSRA
byte keep_ADCSRA;
int VCC;
uint32_t usedFuel, TMP_ODO_usedFuel = 0, usedFuelCost, FuelCost100;
uint32_t TMP_ODO_passed_seconds = 0, TMP_ODO_passed_distance = 0, TMP_ODO_avg_fuel_h = 0, TMP_ODO_avg_fuel_100 = 0, TMP_ODO_passed_speed_ticks = 0, ODO_passed_seconds = 0, ODO_passed_speed_ticks = 0;
uint64_t TMP_ODO_passed_inj_ticks = 0, ODO_passed_inj_ticks = 0;

byte grosze;

byte ignState = 0;
byte dwukropek = 0;
int tempIn;
int tempOut = MIN_TEMP;
unsigned int fuelMeter;
unsigned int Vaccu;
byte key = 0;
struct ts t;
byte screen = 0;
byte setupScreen = 0;
unsigned long lastdisplay = 0;
long fuelLevel;
unsigned long remainingDistance;
unsigned int passed_distance_10m = 0;

volatile uint32_t passed_seconds = 0, passed_speed_ticks = 0; // max 4294967295 ~858993km, 1193046,47h
volatile uint16_t clock_ticks = 0, inj_ticks = 0, m_inj_ticks = 0;
volatile uint16_t m_speed_m, m_speed_km, avg_speed_m, avg_speed_km;
volatile uint16_t m_fuel_h, m_fuel_100, avg_fuel_h, avg_fuel_100, speed_ticks, ODO_avg_fuel_100;
volatile uint32_t passed_distance;
volatile uint64_t passed_inj_ticks = 0;

#ifdef DEBUG
  volatile uint16_t debug_tcnt1;
#endif

/*
 * Cuts last p digits of the value
 */
#define CUT(val,mult) ((uint16_t)((val)/power(10,(uint8_t)mult)))
/*
 * The most calculations in this program made on integer values.
 * Floating point precision is achieved by doing calculations on
 * values multiplied with 10^x, where i x is precision. For example
 * if we want to write 2.345 with precision of 3 digits after
 * floating point, we will use 2345, as it is equal to 2.345*10^3
 * Recalculation is done just before displayng the values.
 * The next three macros are provided to made such calculations.
 *
 * Parameters:
 *    <val>   - value on which calculations are made
 *    <mult>  - multiplier
 *    <prec>  - precision of the calculations; shoud be less
 *              or equal then multiplier
 *
 * ROUND macro just do the rounding to <prec> digits after
 * imaginable point. Be aware that it does not truncate the rest
 * digits
 *
 * uint32_t gives us 4294967 km 296 m of maximium pass
 */
#define ROUND(val,mult,prec) ((uint32_t)(val+5*power(10,(uint8_t)mult-prec-1)))

/*
 * This macro just truncates digits after the floating point, so it
 * only left integer part before.
 */
#define ROUND1(val,mult,prec) ((uint16_t)CUT(ROUND(val,mult,prec),mult))

/*
 * And the following macro truncate digits before floating point,
 * so it only left floating part with the specified precision.
 */
#define ROUND2(val,mult,prec) (uint16_t)(CUT(ROUND(val,mult,prec)-CUT(ROUND(val,mult,prec),mult)*power(10,(uint8_t)mult),mult-prec))

