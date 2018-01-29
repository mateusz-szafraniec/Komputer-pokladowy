void czasDo100() {
  msDo100 = EEPROMReadlong(MSDO100_ADDR);
  if (msDo100 == 0xFFFFFFFF) {
    msDo100 = 0;
  }

  if (msDo100 > 0) {
    lcd.clear();
    lcd.print(F("ostatni pomiar:"));
    lcd.setCursor(0, 1);
    lcd.print(ROUND1(msDo100, 3, 2));
    lcd.print(".");
    lcd.print(ROUND2(msDo100, 3, 2));
    lcd.print("s");
    delay(5000);
  }

  lcd.clear();
  key = 0;

  if (m_speed_km > 0) {
    lcd.print(F("Zatrzymaj auto !"));
    lcd.setCursor(0, 1);
    lcd.print(F("           stop>"));
  }

  while (m_speed_km > 0) {
    key = readKey();
    if (key == DOWNLONG) return;
  }

  lcd.clear();
  lcd.print(F(" !!! START !!!"));
  lcd.setCursor(0, 1);
  lcd.print(F("           stop>"));

  while (m_speed_km == 0) {
    key = readKey();
    if (key == DOWNLONG) return;
  }

  unsigned long poczatekPomiaru = millis();
  lcd.setCursor(0, 0);
  lcd.print(F("                "));

  while (m_speed_km < 100) {
    if (((poczatekPomiaru - millis()) / 1000) % 2) {
      lcd.setCursor(0, 0);
      lcd.print(m_speed_km);
      lcd.print(F("km/h"));
    }
    key = readKey();
    if (key == DOWNLONG) return;
  }

  msDo100 = millis() - poczatekPomiaru;

  lcd.clear();
  EEPROMWritelong(MSDO100_ADDR, msDo100);
  lcd.print(msDo100);
  lcd.setCursor(0, 1);
  lcd.print(ROUND1(msDo100, 3, 2));
  lcd.print(".");
  lcd.print(ROUND2(msDo100, 3, 2));
  lcd.print("s");
  printWyj();
  while (1) {
    key = readKey();
    if (key == DOWNLONG) return;
  }
}

void czasNaCwiartke() {

  msNaCwiartke = EEPROMReadlong(MSNACWIARTKE_ADDR);
  if (msNaCwiartke == 0xFFFFFFFF) {
    msNaCwiartke = 0;
  }

  if (msNaCwiartke > 0) {
    lcd.clear();
    lcd.print(F("ostatni pomiar:"));
    lcd.setCursor(0, 1);
    lcd.print(ROUND1(msNaCwiartke, 3, 2));
    lcd.print(".");
    lcd.print(ROUND2(msNaCwiartke, 3, 2));
    lcd.print("s");
    delay(5000);
  }

  lcd.clear();
  key = 0;

  if (m_speed_km > 0) {
    lcd.print(F("Zatrzymaj auto !"));
    lcd.setCursor(0, 1);
    lcd.print(F("           stop>"));
  }

  while (m_speed_km > 0) {
    key = readKey();
    if (key == DOWNLONG) return;
  }

  lcd.clear();
  lcd.print(F("  !!! START !!!"));
  lcd.setCursor(0, 1);
  lcd.print(F("           stop>"));

  while (m_speed_km == 0) {
    key = readKey();
    if (key == DOWNLONG) return;
  }
  lcd.setCursor(0, 0);
  lcd.print(F("                "));


  unsigned long poczatekPomiaru = millis();
  unsigned long poczatkowyDystans = passed_distance;

  while ((passed_distance - poczatkowyDystans) < 402) {
    if (((poczatekPomiaru - millis()) / 1000) % 2) {
      lcd.setCursor(0, 0);
      lcd.print(passed_distance - poczatkowyDystans);
      lcd.print("m ");
      lcd.print((int)(((double)(passed_distance - poczatkowyDystans) / (double)402) * 100));
      lcd.print("%");
    }
    key = readKey();
    if (key == DOWNLONG) return;
  }

  msNaCwiartke = millis() - poczatekPomiaru;
  EEPROMWritelong(MSNACWIARTKE_ADDR, msNaCwiartke);
  lcd.clear();
  lcd.print(ROUND1(msNaCwiartke, 3, 2));
  lcd.print(".");
  lcd.print(ROUND2(msNaCwiartke, 3, 2));
  lcd.print("s");
  printWyj();

  while (1) {
    key = readKey();
    if (key == DOWNLONG) return;
  }
}

void clearData() { // kasowanie danych
  //powiększenie liczników całkowitych
  ODO_passed_seconds = ODO_passed_seconds + passed_seconds;
  ODO_passed_speed_ticks = ODO_passed_speed_ticks + passed_speed_ticks;
  ODO_passed_inj_ticks = ODO_passed_inj_ticks + passed_inj_ticks;

  saveODOToEEPROM();

  passed_seconds = 0;
  passed_speed_ticks = 0;
  passed_distance = 0;
  passed_inj_ticks = 0;

  avg_fuel_h = 0;
  avg_fuel_100 = 0;
  avg_speed_m = 0;
  avg_speed_km = 0;

  clock_ticks = 0;
  inj_ticks = 0;

  m_speed_m = 0;
  m_speed_km = 0;
  m_fuel_h = 0;
  m_fuel_100 = 0;
  speed_ticks = 0;
  TCNT1 = 0;

  saveDataToEEPROM();
}

void softClear() { //kasowanie spalania chwilowego
  clock_ticks = 1;
  inj_ticks = 0;

  m_speed_m = 0;
  m_speed_km = 0;
  m_fuel_h = 0;
  m_fuel_100 = 0;
  speed_ticks = 0;
  TCNT1 = 0;
}

int readKey() {
  unsigned long mStart = millis();
  int UP = !(digitalRead(PIN_UP));
  int DOWN = !(digitalRead(PIN_DOWN));
  int UPpressed = UP;
  int DOWNpressed = DOWN;

  if (UP or DOWN) { //wciśnięto klawisz
    while (UP or DOWN) {
      UP = !(digitalRead(PIN_UP));
      DOWN = !(digitalRead(PIN_DOWN));
    } //czekaj aż puści klawisz

    unsigned long pressedTime = millis() - mStart;

    if (pressedTime < 1000) { //czas krótszy niż sekunda
      if (pressedTime >= DEBOUNCE) {
        if (UPpressed) {
          delay(DEBOUNCE); return UPSHORT;
        }
        if (DOWNpressed) {
          delay(DEBOUNCE); return DOWNSHORT;
        }
      }
    }
    else { //czas dłuższy niż sekunda
      if (DOWNpressed) {
        delay(DEBOUNCE);  return DOWNLONG;
      }
      else {
        delay(DEBOUNCE);
        return UPLONG;
      }
    }
  } else return 0;
}




/***************************************************
 *  Name:        readVcc
 *
 *  Returns:     Vcc level in mV
 *
 *  Parameters:  None.
 *
 *  Description: Measures Vcc of ATMega
 *
 ***************************************************/
void readVcc() {
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  VCC = ADCL;
  VCC |= ADCH << 8;
  VCC = 1126400L / VCC; // Back-calculate AVcc in mV
}

void readVaccu() {
  analogReference (INTERNAL); // Sets the internal 1.1 volt reference.
  delay(2); // Wait for Vref to settle
  analogRead(PIN_ACCU);
  Vaccu = map(analogRead(PIN_ACCU), 0, 1023, 0, MAX_VOLTAGE);
}


/***************************************************
 *  Name:        freeRam
 *
 *  Returns:     amount of free RAM (bytes)
 *
 *  Parameters:  None.
 *
 *  Description: Measures free RAM
 *
 ***************************************************/
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

