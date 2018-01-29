void enterINJ_FLOW() {
  byte lastINJ_FLOW = INJ_FLOW;
  byte o_ile = 1;
  lcd.print(F("przeplyw    x10>"));
  while (1) {
    printWyj();
    lcd.print(INJ_FLOW);
    uint32_t usedFuel;
    usedFuel = INJ_FLOW * RATIO * passed_inj_ticks / 32768 / 30;
    unsigned int ml;
    ml = ROUND2(usedFuel, 3, 2);
    lcd.print(" ");
    lcd.print(ROUND1(usedFuel, 3, 2));
    lcd.print(".");
    if (ml < 10) lcd.print("0");
    lcd.print(ml);
    lcd.print("l");
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      INJ_FLOW = lastINJ_FLOW;
      return;
    }
    if (key == UPLONG) {
      lcd.clear();

      if (o_ile == 1) {
        o_ile = 10;
        lcd.print(F("przeplyw  zapis>"));
      } else o_ile = 1;

      if (o_ile == 1) {
        EEPROM.write(INJ_FLOW_ADDR, INJ_FLOW);
        return;
      }
    }
    if (key == UPSHORT) INJ_FLOW += o_ile;
    if (key == DOWNSHORT) INJ_FLOW -= o_ile;
  }
}

void enterSPEED_TICKS() {
  unsigned int lastSPEED_TICKS = SPEED_TICKS;
  unsigned int o_ile = 1;
  lcd.print(F("impulsy     x10>"));
  while (1) {
    printWyj();
    lcd.print(SPEED_TICKS);
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      SPEED_TICKS = lastSPEED_TICKS;
      return;
    }
    if (key == UPLONG) {
      lcd.clear();
      if (o_ile == 1) {
        o_ile = 10;
        lcd.print(F("impulsy    x100>"));
      }
      else {
        if (o_ile == 10) {
          o_ile = 100;
          lcd.print(F("impulsy   x1000>"));
        } else {
          if (o_ile == 100) {
            o_ile = 1000;
            lcd.print(F("impulsy   zapis>"));
          } else if (o_ile == 1000) {
            EEPROMWriteInt(SPEED_TICKS_ADDR, SPEED_TICKS);
            return;
          }
        }
      }
    }
    if (key == UPSHORT) SPEED_TICKS += o_ile;
    if (key == DOWNSHORT) SPEED_TICKS -= o_ile;
  }
}

void enterTANK_SIZE() {
  byte lastTANK_SIZE = TANK_SIZE;
  lcd.print(F("zbiornik  zapis>"));
  while (1) {
    printWyj();
    lcd.print(TANK_SIZE);
    lcd.print("L");
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      TANK_SIZE = lastTANK_SIZE;
      return;
    }
    if (key == UPLONG) {
      EEPROM.write(TANK_SIZE_ADDR, TANK_SIZE);
      return;
    }
    if (key == UPSHORT) TANK_SIZE++;
    if (key == DOWNSHORT) TANK_SIZE--;
  }
}

void enterRATIO() {
  byte lastRATIO = RATIO;
  lcd.print(F("wspolczynnik OK>"));
  while (1) {
    printWyj();
    lcd.print(RATIO);
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      RATIO = lastRATIO;
      return;
    }
    if (key == UPLONG) {
      EEPROM.write(RATIO_ADDR, RATIO);
      return;
    }
    if (key == UPSHORT) {
      if (RATIO >= 8) {
        RATIO = 0;
      }
      RATIO++;
    }

    if (key == DOWNSHORT) {
      if (RATIO < 2) RATIO = 9;
      RATIO--;
    }
  }
}

void clearODO() {
  ODO_passed_seconds = 0;
  ODO_passed_speed_ticks = 0;
  ODO_passed_inj_ticks = 0;
  DOTANKOWANIE = 0;
  saveODOToEEPROM();
}

void ODOtoTrip() {
  passed_seconds += ODO_passed_seconds;
  passed_speed_ticks += ODO_passed_speed_ticks;
  passed_inj_ticks += ODO_passed_inj_ticks;

  saveDataToEEPROM();
  clearODO();
}

void setupClock() {
  lcd.print(F("minuty    dalej>"));
  printWyj();
  DS3231_get(&t); //Get time
  if (t.hour < 10) lcd.print("0");
  lcd.print(t.hour);
  lcd.print(":");
  if (t.min < 10) lcd.print("0");
  lcd.print(t.min);
  lcd.setCursor(3, 1);
  lcd.blink();
  while (1) {
    key = 0;
    while (key == 0) {
      key = readKey();
    }

    if (key == UPSHORT) {
      t.min++;
      if (t.min > 59) t.min = 0;
      lcd.setCursor(3, 1);
      if (t.min < 10) lcd.print("0");
      lcd.print(t.min);
      lcd.setCursor(3, 1);
    }
    if (key == DOWNSHORT) {
      if (t.min == 0) t.min = 60;
      t.min--;
      lcd.setCursor(3, 1);
      if (t.min < 10) lcd.print("0");
      lcd.print(t.min);
      lcd.setCursor(3, 1);
    }
    if (key == DOWNLONG) return;

    if (key == UPLONG) {
      setHour();
      return;
    }
  }
}

void setHour() {
  lcd.setCursor(0, 0);
  lcd.print(F("godz.    zapisz>"));
  lcd.setCursor(1, 1);
  while (1) {
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == UPSHORT) {
      t.hour++;
      if (t.hour > 23) t.hour = 0;
      lcd.setCursor(0, 1);
      if (t.hour < 10) lcd.print("0");
      lcd.print(t.hour);
      lcd.setCursor(1, 1);
    }
    if (key == DOWNSHORT) {
      if (t.hour == 0) t.hour = 24;
      t.hour--;
      lcd.setCursor(0, 1);
      if (t.hour < 10) lcd.print("0");
      lcd.print(t.hour);
      lcd.setCursor(1, 1);
    }
    if (key == DOWNLONG) return;

    if (key == UPLONG) {
      DS3231_set(t);
      return;
    }
  }
}

void enterFUEL_PRICE() {
  unsigned int lastFUEL_PRICE = FUEL_PRICE;
  unsigned int o_ile = 1;
  lcd.print(F("cena paliwa x10>"));
  while (1) {
    printWyj();
    lcd.print(ROUND1(FUEL_PRICE, 2, 2));
    lcd.print(".");
    byte grosze = ROUND2(FUEL_PRICE, 2, 2);
    if (grosze < 10) lcd.print("0");
    lcd.print(grosze);
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      FUEL_PRICE = lastFUEL_PRICE;
      return;
    }
    if (key == UPLONG) {
      lcd.clear();
      if (o_ile == 1) {
        o_ile = 10;  //
        lcd.print(F("cena pal.  x100>"));
      }
      else {
        if (o_ile == 10) {
          o_ile = 100;         //
          lcd.print(F("cena pal. zapis>"));
        } else if (o_ile == 100) {
          EEPROMWriteInt(FUEL_PRICE_ADDR, FUEL_PRICE);
          return;
        }
      }
    }
    if (key == UPSHORT) FUEL_PRICE += o_ile;
    if (key == DOWNSHORT) FUEL_PRICE -= o_ile;
  }
}

void calibrateVAccu() {
  unsigned int lastMAX_VOLTAGE = MAX_VOLTAGE;
  lcd.print(F("VAccu      zapis>"));
  while (1) {
    printWyj();
    readVcc();
    readVaccu();
    lcd.print(ROUND1(Vaccu, 3, 2));
    lcd.print(".");
    lcd.print(ROUND2(Vaccu, 3, 2));
    lcd.print("V");
    lcd.print(";");
    lcd.print(MAX_VOLTAGE);
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      MAX_VOLTAGE = lastMAX_VOLTAGE;
      return;
    }
    if (key == UPLONG) {
      EEPROMWriteInt(MAX_VOLTAGE_ADDR, MAX_VOLTAGE);
      return;
    }
    if (key == UPSHORT) MAX_VOLTAGE=MAX_VOLTAGE+5;
    if (key == DOWNSHORT) MAX_VOLTAGE=MAX_VOLTAGE-5;
  }
}

void dotankowanie(){
  unsigned int lastDOTANKOWANIE = DOTANKOWANIE;
  byte o_ile = 1;           //
  lcd.print(F("ile litrow  x10>"));
  while (1) {
    printWyj();
    lcd.print(DOTANKOWANIE);
    lcd.print(" ");
    lcd.print("l");
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) {
      DOTANKOWANIE = lastDOTANKOWANIE;
      return;
    }
    if (key == UPLONG) {
      lcd.clear();

      if (o_ile == 1) {
        o_ile = 10;               //
        lcd.print(F("dotankow. zapis>"));
      } else o_ile = 1;

      if (o_ile == 1) {
        EEPROMWriteInt(DOTANKOWANIE_ADDR, DOTANKOWANIE);
        return;
      }
    }
    if (key == UPSHORT) DOTANKOWANIE += o_ile;
    if (key == DOWNSHORT) DOTANKOWANIE -= o_ile;
  }
}

