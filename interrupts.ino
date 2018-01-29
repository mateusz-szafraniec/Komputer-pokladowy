/***************************************************
 *  Name:        sleepInterrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt
 *
 ***************************************************/
void sleepInterrupt(void)
{
  /* This will bring us back from sleep. */

  /* We detach the interrupt to stop it from
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(1);
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  
  detachInterrupt(0); //wyłączamy przerwanie od RTC
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(1, sleepInterrupt, LOW); //wybudzenie stanem niskim

  digitalWrite(PIN_PWR, LOW); //wyłączamy zasilanie
  digitalWrite(SCK, LOW); //wyłączamy LED

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // shut down ADC
  keep_ADCSRA = ADCSRA;
  ADCSRA = 0;

#ifdef DEEP_SLEEP
  // turn off various modules
  power_all_disable ();

  noInterrupts ();           // timed sequence follows
  sleep_enable();
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);
  interrupts ();             // guarantees next instruction executed
  sleep_cpu();
#endif

#ifndef DEEP_SLEEP
  sleep_enable();
  sleep_mode();
#endif

  /* The program will continue from here. */

  /* First thing to do is disable sleep. */
  sleep_disable();

#ifdef DEEP_SLEEP
  power_all_enable ();
#endif

  ADCSRA = keep_ADCSRA;
  digitalWrite(PIN_PWR, HIGH);
  delay(250);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  myLcdInit();

#ifndef OLED
  lcd.display();
  lcd.backlight();
#endif

  softClear();
  readDataFromEEPROM();
  passed_distance = passed_speed_ticks * 1000 / SPEED_TICKS; //obliczenie przebytego dystansu
  attachInterrupt(0, int0, FALLING); //ustawiamy przerwanie od RTC
}

/***************************************************
 *  Name:        int0
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine 32kHz inerrupt
 *
 ***************************************************/
void int0()
{
  if (!digitalRead(PIN_INJ)) inj_ticks++; //mamy niski stan, zliczamy czas wtrysku

  if (!clock_ticks) //zegar wskazuje 0
  {
    digitalWrite(SCK, !(digitalRead(SCK))); //miganie ledem

    speed_ticks = TCNT1;
    TCNT1 = 0; //kasowanie licznika drogi

    m_inj_ticks = inj_ticks; //czas wtrysku na sekundę do debugowania i ekonozmizera

#ifdef DEBUG
    debug_tcnt1 = speed_ticks;
#endif

#ifdef COUNT_ABOVE
    if (speed_ticks < COUNT_ABOVE) {
      speed_ticks = 0;
    }
#endif

    passed_speed_ticks += speed_ticks;
    passed_distance = passed_speed_ticks * (uint64_t)1000 / SPEED_TICKS;

    m_speed_m    = calc_speed_m(speed_ticks, 1);
    m_speed_km   = m_speed_m * 36 / 10;

    if ((inj_ticks == 0) or (inj_ticks >= MAX_INJ_TICKS)) { // jeśli czas wtrysku =0 lub dłuższy od max - nie zliczamy go
      inj_ticks = 0;
      m_fuel_h = 0;
      m_fuel_100 = 0;
      if (m_speed_km > 4) passed_seconds++; //hamowanie slinikiem lub silnik zgaszony - zliczamy czas jazdy
    }
    if (inj_ticks > 0) {
      passed_inj_ticks += inj_ticks;

      m_fuel_h     = calc_fuel_h(inj_ticks, 1);
      m_fuel_100   = calc_fuel_100(m_fuel_h, speed_ticks, 1);

      passed_seconds++;
      inj_ticks = 0;
    }
  }

  clock_ticks++;

  if (clock_ticks == 32768) clock_ticks = 0;
}

