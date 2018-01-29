/*
 * Do all the startup-time peripheral initializations.
 */

void setup()
{
#ifdef UART_SPEED
  Serial.begin(UART_SPEED);
#endif

  pinMode(PIN_INT0, INPUT);
  pinMode(PIN_SPEED, INPUT);
  pinMode(PIN_INJ, INPUT);
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_IGN, INPUT);
  pinMode(PIN_PWR, OUTPUT);
  pinMode(SCK, OUTPUT);
  
  INJ_FLOW = EEPROM.read(INJ_FLOW_ADDR);
  SPEED_TICKS = EEPROMReadInt(SPEED_TICKS_ADDR);
  TANK_SIZE = EEPROM.read(TANK_SIZE_ADDR);
  RATIO = EEPROM.read(RATIO_ADDR);
  FUEL_PRICE = EEPROMReadInt(FUEL_PRICE_ADDR);
  MAX_VOLTAGE = EEPROMReadInt(MAX_VOLTAGE_ADDR);
  DOTANKOWANIE = EEPROMReadInt(DOTANKOWANIE_ADDR);

#ifdef UART_SPEED
  Serial.println(INJ_FLOW);
  Serial.println(SPEED_TICKS);
  Serial.println(FULL_TANK);
  Serial.println(EMPTY_TANK);
  Serial.println(TANK_SIZE);
  Serial.println(RATIO);
#endif

  if (INJ_FLOW == 0xFF) {
    INJ_FLOW = INJ_FLOW_DEFAULT;
    EEPROM.write(INJ_FLOW_ADDR, INJ_FLOW_DEFAULT);
  }
  if (SPEED_TICKS == 0xFFFF) {
    SPEED_TICKS = SPEED_TICKS_DEFAULT;
    EEPROMWriteInt(SPEED_TICKS_ADDR, SPEED_TICKS_DEFAULT);
  }
  if (TANK_SIZE == 0xFF) {
    TANK_SIZE = TANK_SIZE_DEFAULT;
    EEPROM.write(TANK_SIZE_ADDR, TANK_SIZE_DEFAULT);
  }
  if (RATIO == 0xFF) {
    RATIO = RATIO_DEFAULT;
    EEPROM.write(RATIO_ADDR, RATIO_DEFAULT);
  }
  if (FUEL_PRICE == 0xFFFF) {
    FUEL_PRICE = FUEL_PRICE_DEFAULT;
    EEPROMWriteInt(FUEL_PRICE_ADDR, FUEL_PRICE_DEFAULT);
  }
  if (MAX_VOLTAGE == 0xFFFF) {
    MAX_VOLTAGE = MAX_VOLTAGE_DEFAULT;
    EEPROMWriteInt(MAX_VOLTAGE_ADDR, MAX_VOLTAGE_DEFAULT);
  }
    if (DOTANKOWANIE == 0xFFFF) {
    DOTANKOWANIE = 0;
    EEPROMWriteInt(DOTANKOWANIE_ADDR, 0);
  }
  readDataFromEEPROM();

  digitalWrite(PIN_PWR, HIGH);
  delay(250);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  //memset(recv, 0, BUFF_MAX);
  sensors.begin();
  sensors.setWaitForConversion(false); // ustawiamy aby sensory pobierały wartości w tle, nie zawieszając programu
  //sensors.setResolution(12); // ustalamy rozdzielczość pomiaru
  sensors.requestTemperatures(); // wysyłamy pierwsze żądanie odczytu temperatur
  myLcdInit();

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TIMSK1 = 0;
  TCNT1  = 0;//initialize counter value to 0

  // External clock source on T1 pin. Clock on RISING edge.
  //TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);

  // External clock source on T1 pin. Clock on FALLING edge.
  TCCR1B |= (1 << CS12) | (1 << CS11);

  TCCR0A = 0; //PLACEBO ???
  sei(); // enable all interrupts

#ifndef OLED
  lcd.backlight();
#endif

#ifdef UART_SPEED
  Serial.print(F("Setup done");
#endif
  analogReference(INTERNAL);
  attachInterrupt(0, int0, FALLING); //ustawiamy przerwanie od RTC
}

