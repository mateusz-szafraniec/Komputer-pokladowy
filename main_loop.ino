void loop()
{
  ignState = !(digitalRead(PIN_IGN));
  if (!ignState) { //stacyjka wyłączona
    delay(5000);
    ignState = !(digitalRead(PIN_IGN));
    if (!ignState) { //stacyjka wyłączona
      saveDataToEEPROM();
#ifndef OLED
      lcd.noBacklight();
      lcd.noDisplay();
#endif
#ifdef OLED
      lcd.clear();
#endif
      enterSleep(); //idziemy spać
#ifdef WELCOME_MESSAGE
      //lcd.clear();
      lcd.print(F(WELCOME_MESSAGE));
      lcd.setCursor(0, 1);
      lcd.print(F(VERSION));
      delay(2000);
      lcd.clear();
#endif
    }
  }

  int key = readKey();

  if (key > 0) {

    if (key == UPSHORT) {
      screen++;
      if (screen > SCREEN_COUNT) screen = 0;
    }
    if (key == DOWNSHORT) {
      if (screen == 0) screen = SCREEN_COUNT + 1;
      screen--;
    }

    if (screen == SCREEN_ACCELERATION) { //pomiar czasu
      if (key == DOWNLONG) {
        czasNaCwiartke();
      }
      if (key == UPLONG) {
        czasDo100();
      }
    }
    if (screen == SCREEN_SETTINGS) { //ekran konfiguracji
      if (key == DOWNLONG) {
        clearData(); //kasujemy dane
        clearODO();
        screen = SCREEN_TRIP;
      }
      if (key == UPLONG) {
        runSetup();
       }
    }
    lcd.clear();
    lastdisplay = 0;
  }

  if ((millis() - lastdisplay) > REFRESH_RATE) {
    displayData();
    lastdisplay = millis();
  }
  if (screen == SCREEN_CLOCK) Dwukropek(); //miganie dwukropkiem

}

