void runSetup() {
  while (1) {
    lcd.clear();
    printWyj();
    lcd.setCursor(0, 0);
    if (setupScreen == 0xFF) setupScreen = SETUP_SCREEN_COUNT;
    if (setupScreen > SETUP_SCREEN_COUNT) setupScreen = 0;

    switch (setupScreen) {
      case SETUP_SCREEN_CLOCK: // zegarek
        lcd.print(F("zegar     ustaw>"));
        break;

      case SETUP_SCREEN_INJ_FLOW: // ustawienie przepływu wtryskiwacza
        lcd.print(F("wtryskiwacz    >"));
        break;

      case SETUP_SCREEN_SPEED_TICKS: // ustawienie ilości impulsów na kilometr
        lcd.print(F("imp./km        >"));
        break;

      case SETUP_SCREEN_TANK_SIZE: // ustawienie rozmiaru baku
        lcd.print(F("pojemnosc baku >"));
        break;

      case SETUP_SCREEN_RATIO: // ustawienie współczynnika wtrysku
        lcd.print(F("wspolczynnik   >"));
        break;

      case SETUP_SCREEN_FUEL_PRICE: // ustawienie ceny paliwa
        lcd.print(F("cena paliwa    >"));
        break;

      case SETUP_SCREEN_CLEAR_TRIP: // kasowanie TRIP
        lcd.print(F("kasuj TRIP     >"));
        break;

      case SETUP_SCREEN_ODO_TO_TRIP: //  ODO -> TRIP
        lcd.print(F("kopiuj ODO>Trip>"));
        break;

      case SETUP_SCREEN_CALIBRATE_VACCU: // Kalibracja VAccu
        lcd.print(F("kalibr.VAccu   >"));
        break;
      case SETUP_SCREEN_DOTANKOWANIE: // Dotankowanie
        lcd.print(F("dotankowanie   >"));
        break;
    }
    key = 0;
    while (key == 0) {
      key = readKey();
    }
    if (key == DOWNLONG) return;
    if (key == UPSHORT) setupScreen++;
    if (key == DOWNSHORT) setupScreen--;
    if (key == UPLONG) {
      lcd.clear();
      switch (setupScreen) {
        case SETUP_SCREEN_CLOCK: // zegarek
          setupClock();
          lcd.noBlink();
          break;

        case SETUP_SCREEN_INJ_FLOW: // ustawienie przepływu wtryskiwacza
          enterINJ_FLOW();
          break;

        case SETUP_SCREEN_SPEED_TICKS: // ustawienie ilości impulsów na kilometr
          enterSPEED_TICKS();
          break;

        case SETUP_SCREEN_TANK_SIZE: // ustawienie rozmiaru baku
          enterTANK_SIZE();
          break;

        case SETUP_SCREEN_RATIO: // ustawienie współczynnika wtrysku
          enterRATIO();
          break;

        case SETUP_SCREEN_FUEL_PRICE: // ustawienie ceny paliwa
          enterFUEL_PRICE();
          break;

        case SETUP_SCREEN_CLEAR_TRIP: // kasowanie TRIP
          clearData();
          break;

        case SETUP_SCREEN_ODO_TO_TRIP: //  ODO -> TRIP
          ODOtoTrip();
          break;

        case SETUP_SCREEN_CALIBRATE_VACCU: // Kalibracja VAccu
          calibrateVAccu();
          break;
        case SETUP_SCREEN_DOTANKOWANIE: // Kalibracja VAccu
          dotankowanie();
          break;
      }
    }
  }
}

