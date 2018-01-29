void displayData() {

  avg_speed_m  = calc_speed_m(passed_speed_ticks, passed_seconds);
  avg_speed_km = (avg_speed_m * 36) / 10;

  avg_fuel_h   = calc_fuel_h(passed_inj_ticks, passed_seconds);
  avg_fuel_100 = calc_fuel_100(avg_fuel_h, passed_speed_ticks, passed_seconds);

  switch (screen) {
    case SCREEN_CLOCK: // zegarek
      {
        float tempInF;
        DS3231_get(&t); //Get time
        tempInF = DS3231_get_treg(); //Get temperature
        if (tempInF > 0) tempInF += 0.5;
        else tempInF -= 0.5;
        tempIn = (int)tempInF;
        
        if (sensors.getDeviceCount()>1){
          DeviceAddress secondSensorAddress;
          sensors.getAddress(secondSensorAddress,1);
        if (sensors.isConversionComplete()) {
          tempInF = sensors.getTempCByIndex(1); // pobranie wartości drugiego termometru
          if (tempInF == 0) tempInF = MIN_TEMP;
          sensors.requestTemperatures(); // wysłanie kolejnego żądania temperatur
          if ((tempInF > MIN_TEMP) & (tempInF < MAX_TEMP)) {
            if (tempInF > 0) tempInF += 0.5;
            else tempInF -= 0.5;
            tempIn = (int)tempInF;
          }
        }
        }

        if (sensors.isConversionComplete()) {
          float tempOutF = sensors.getTempCByIndex(0); // pobranie wartości pierwszego termometru
          if (tempOutF == 0) tempOutF = MIN_TEMP;
          sensors.requestTemperatures(); // wysłanie kolejnego żądania temperatur
          if ((tempOutF > MIN_TEMP) & (tempOutF < MAX_TEMP)) {
            if (tempOutF > 0) tempOutF += 0.5;
            else tempOutF -= 0.5;
            tempOut = (int)tempOutF;
          }
        }
        displayBigClock();

      }
      break;

    case SCREEN_MOMENTARY:
      prepareSpace(0, 0, 3);
      lcd.print(m_speed_km);
      lcd.setCursor(5, 0);
      lcd.print(F("km/h"));
      prepareSpace(0, 1, 4);
      if (m_speed_km > 5) {
        lcd.print(ROUND1(m_fuel_100, 3, 1));
        lcd.print(".");
        lcd.print(ROUND2(m_fuel_100, 3, 1));
        lcd.setCursor(5, 1);
        lcd.print(F("l/100km"));
      } else {
        lcd.print(ROUND1(m_fuel_h, 3, 1));
        lcd.print(".");
        lcd.print(ROUND2(m_fuel_h, 3, 1));
        lcd.setCursor(5, 1);
        lcd.print(F("l/h    "));
      }
      break;

    case SCREEN_AVERAGE:
      lcd.setCursor(0, 0);
      lcd.print(F("sr."));
      prepareSpace(4, 0, 4);
      lcd.print(avg_speed_km);
      lcd.setCursor(9, 0);
      lcd.print(F("km/h"));
      lcd.setCursor(0, 1);
      lcd.print(F("sr."));
      prepareSpace(4, 1, 4);
      lcd.print(ROUND1(avg_fuel_100, 3, 1));
      lcd.print(".");
      lcd.print(ROUND2(avg_fuel_100, 3, 1));
      lcd.setCursor(9, 1);
      lcd.print(F("l/100km"));
      break;

    case SCREEN_TRIP:
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)(passed_seconds / 3600));
      lcd.print(":");
      if (((passed_seconds % 3600) / 60) < 10) lcd.print("0");
      lcd.print((uint8_t)((passed_seconds % 3600) / 60));
      lcd.print(":");
      if ((passed_seconds % 60) < 10) lcd.print("0");
      lcd.print((uint8_t)(passed_seconds % 60));
      lcd.setCursor(0, 1);
      lcd.print(ROUND1(passed_distance, 3, 2));
      if (passed_distance < 1000000)
      {
        lcd.print(".");
        passed_distance_10m = ROUND2(passed_distance, 3, 2);
        if (passed_distance_10m < 10) lcd.print("0");
        lcd.print(passed_distance_10m);
      }
      lcd.print(F("km "));
      usedFuel = INJ_FLOW * RATIO * passed_inj_ticks / 32768 / 30;
      unsigned int _10ml;
      _10ml = ROUND2(usedFuel, 3, 2);
      lcd.print(ROUND1(usedFuel, 3, 2));
      if (usedFuel < 100000)
      {
        lcd.print(".");
        if (_10ml < 10) lcd.print("0");
        lcd.print(_10ml);
      }
      lcd.print("l");
      break;

    case SCREEN_COSTS:
      lcd.setCursor(0, 0);
      usedFuel = INJ_FLOW * RATIO * passed_inj_ticks / 32768 / 30;
      usedFuelCost = usedFuel * (uint32_t)FUEL_PRICE;
      grosze = ROUND2(usedFuelCost, 5, 2);
      lcd.print(F("koszt:"));
      lcd.print(ROUND1(usedFuelCost, 5, 2));
      lcd.print(".");
      if (grosze < 10) lcd.print("0");
      lcd.print(grosze);
      lcd.print(F(" zl"));
      lcd.setCursor(0, 1);
      FuelCost100 = (uint32_t)avg_fuel_100 * (uint32_t)FUEL_PRICE;
      grosze = ROUND2(FuelCost100, 7, 2);
      lcd.print(F("1km:"));
      lcd.print(ROUND1(FuelCost100, 7, 2));
      lcd.print(".");
      if (grosze < 10) lcd.print("0");
      lcd.print(grosze);
      lcd.print(F(" zl"));
      break;

    case SCREEN_ODO:

      TMP_ODO_passed_seconds = ODO_passed_seconds + passed_seconds;
      TMP_ODO_passed_distance = (ODO_passed_speed_ticks * (uint64_t)1000 / SPEED_TICKS) + passed_distance;
      TMP_ODO_passed_inj_ticks = ODO_passed_inj_ticks + passed_inj_ticks;
      TMP_ODO_passed_speed_ticks = ODO_passed_speed_ticks + passed_speed_ticks;

      TMP_ODO_avg_fuel_h   = calc_fuel_h(TMP_ODO_passed_inj_ticks, TMP_ODO_passed_seconds);
      TMP_ODO_avg_fuel_100 = calc_fuel_100(TMP_ODO_avg_fuel_h, TMP_ODO_passed_speed_ticks, TMP_ODO_passed_seconds);

      lcd.setCursor(0, 0);
      lcd.print(F("ODO:"));
      lcd.print((uint8_t)(TMP_ODO_passed_seconds / 3600));
      lcd.print(":");
      if (((TMP_ODO_passed_seconds % 3600) / 60) < 10) lcd.print("0");
      lcd.print((uint8_t)((TMP_ODO_passed_seconds % 3600) / 60));

      lcd.print(" ");
      lcd.print(ROUND1(TMP_ODO_avg_fuel_100, 3, 1));
      lcd.print(".");
      lcd.print(ROUND2(TMP_ODO_avg_fuel_100, 3, 1));
      lcd.print(F("l/100km"));

      lcd.setCursor(0, 1);
      lcd.print(ROUND1(TMP_ODO_passed_distance, 3, 0));
      lcd.print(F("km "));

      TMP_ODO_usedFuel = INJ_FLOW * RATIO * TMP_ODO_passed_inj_ticks / 32768 / 30;
      lcd.print(ROUND1(TMP_ODO_usedFuel, 3, 0));
      lcd.print("l");

      break;

    case SCREEN_ACCELERATION:
      lcd.setCursor(0, 0);
      lcd.print(F("czas do 100km/h>"));
      lcd.setCursor(0, 1);
      lcd.print(F("czas na 1/4mili>"));
      break;

    case SCREEN_VOLTAGE:
      readVcc();
      readVaccu();
      fuelMeter = analogRead(PIN_FUEL_METER);
      lcd.setCursor(0, 0);
      lcd.print(F("aku:"));
      prepareSpace(4, 0, 6);
      lcd.print(ROUND1(Vaccu, 3, 2));
      lcd.print(".");
      if ((ROUND2(Vaccu, 3, 2)) <10) lcd.print("0");
      lcd.print(ROUND2(Vaccu, 3, 2));
      lcd.setCursor(9, 0);
      lcd.print("v ");
      lcd.print(ROUND1(VCC, 3, 2));
      lcd.print(".");
      if ((ROUND2(VCC, 3, 2)) <10) lcd.print("0");
      lcd.print(ROUND2(VCC, 3, 2));
      lcd.print("v");
      break;
      
    case SCREEN_FUEL_LEVEL:
      TMP_ODO_passed_inj_ticks = ODO_passed_inj_ticks + passed_inj_ticks;
      TMP_ODO_usedFuel = INJ_FLOW * RATIO * TMP_ODO_passed_inj_ticks / 32768 / 30;
      fuelLevel = (DOTANKOWANIE * (unsigned int)10) + ((unsigned int)TANK_SIZE * (unsigned int)10) - (TMP_ODO_usedFuel / 100);
      remainingDistance = ((uint32_t)fuelLevel * (uint32_t)10000) / (uint32_t)avg_fuel_100;
      lcd.setCursor(0, 0);
      lcd.print(F("paliwo:"));
      prepareSpace(7, 0, 4);
      lcd.print(ROUND1(fuelLevel, 1, 1));
      lcd.print(".");
      lcd.print(ROUND2(fuelLevel, 1, 1));
      lcd.print("L");
      lcd.setCursor(0, 1);
      lcd.print(F("pozost:"));
      prepareSpace(7, 1, 4);
      lcd.print(remainingDistance);
      lcd.print("km");
      break;

    case SCREEN_SETTINGS:
      lcd.setCursor(4, 0);
      lcd.print(F("ustawienia >"));
      lcd.setCursor(9, 1);
      lcd.print(F("kasuj >"));
      break;

    case SCREEN_ECONOMIZER:
      lcd.setCursor(0, 0);
      lcd.print(F(". . . . . . . . "));
      lcd.setCursor(0, 1);
      byte bargraphValue;
      bargraphValue = map(m_inj_ticks, 0, MAX_BARGRAPH_INJ_TICKS, 0, 16);
      if ((m_inj_ticks > 0) && (bargraphValue == 0)) bargraphValue = 1;
      bargraph(bargraphValue);
      break;

#ifdef DEBUG
    case SCREEN_DEBUG1:
      lcd.setCursor(0, 0);
      lcd.print(F("INJ:"));
      prepareSpace(4, 0, 10);
      if (m_inj_ticks < 10) lcd.print(" ");
      if (m_inj_ticks < 100) lcd.print(" ");
      if (m_inj_ticks < 1000) lcd.print(" ");
      if (m_inj_ticks < 10000) lcd.print(" ");
      lcd.print(m_inj_ticks);
      prepareSpace(9, 0, 5);
      lcd.print(" ");
      if (debug_tcnt1 < 10) lcd.print(" ");
      if (debug_tcnt1 < 100) lcd.print(" ");
      lcd.print(debug_tcnt1);
      lcd.print(";");
      lcd.print(digitalRead(PIN_SPEED));
      lcd.setCursor(0, 1);
      lcd.print(F("ticks:"));
      prepareSpace(6, 1, 9);
      lcd.print(passed_inj_ticks);
      break;

    case SCREEN_DEBUG2:
      lcd.setCursor(0, 0);
      lcd.print(F("RAM:"));
      prepareSpace(4, 0, 5);
      lcd.print(freeRam());
      prepareSpace(9, 0, 5);
      lcd.print(" ");
      if (debug_tcnt1 < 10) lcd.print(" ");
      if (debug_tcnt1 < 100) lcd.print(" ");
      lcd.print(debug_tcnt1);
      lcd.setCursor(0, 1);
      lcd.print(F("DST:"));
      prepareSpace(4, 1, 9);
      lcd.print((uint32_t)passed_speed_ticks);
      break;
#endif      
  }
}

