// the 8 arrays that form each segment of the custom numbers
byte bar1[8] =
{
  B11100,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11100
};
byte bar2[8] =
{
  B00111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B00111
};
byte bar3[8] =
{
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte bar4[8] =
{
  B11110,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11100
};
byte bar5[8] =
{
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00111
};
byte bar6[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte bar7[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111
};
byte bar8[8] =
{
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void prepareSpace(byte col, byte line, byte Size) {
  lcd.setCursor(col, line);
  for (byte i = 0; i < Size; i++, lcd.print(" "));
  lcd.setCursor(col, line);
}

void custom0(byte col)
{ // uses segments to build the number 0
  if (col != 3) {
    lcd.setCursor(col, 0);
    lcd.write(2);
    lcd.write(8);
    lcd.write(1);
    lcd.setCursor(col, 1);
    lcd.write(2);
    lcd.write(6);
    lcd.write(1);
  }
}

void custom1(byte col)
{
  if (col != 3) {
    lcd.setCursor(col, 0);
    lcd.write(32);
    lcd.write(32);
  } else lcd.setCursor(5, 0);
  lcd.write(1);
  if (col != 3) {
    lcd.setCursor(col, 1);
    lcd.write(32);
    lcd.write(32);
  } else lcd.setCursor(5, 1);
  lcd.write(1);
}

void custom2(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(5);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(6);
}

void custom3(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(5);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void custom4(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(1);
}

void custom5(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void custom6(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
}

void custom7(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(8);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(1);
}

void custom8(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
}

void custom9(byte col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void printNumber(byte value, byte col) {
  if (value == 0) custom0(col);
  if (value == 1) custom1(col);
  if (value == 2) custom2(col);
  if (value == 3) custom3(col);
  if (value == 4) custom4(col);
  if (value == 5) custom5(col);
  if (value == 6) custom6(col);
  if (value == 7) custom7(col);
  if (value == 8) custom8(col);
  if (value == 9) custom9(col);
}

void myLcdInit() {
  #ifdef OLED
  lcd.oled_init();
  #endif
  #ifndef OLED
  lcd.init();
  #endif
  

  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);
  lcd.createChar(6, bar6);
  lcd.createChar(7, bar7);
  lcd.createChar(8, bar8);
  lcd.clear();
}

void displayBigClock() {
  byte hd, h, md, m;

  h = t.hour;
  if (h > 9) {
    hd = (h - (h % 10)) / 10;
    h = h % 10;
  }
  else hd = 0;

  m = t.min;
  if (m > 9) {
    md = (m - (m % 10)) / 10;
    m = m % 10;
  }
  else md = 0;

  prepareSpace(0, 0, 3);
  lcd.print(tempIn);
  if ((hd == 2) & (tempIn > -10)) { //godzina >19, powyżej -10
    lcd.print((char)223);
  } else if (hd < 2) lcd.print((char)223); //temperatura <-10 i godzina <20

  prepareSpace(0, 1, 3);
  if (tempOut <= MIN_TEMP) lcd.print("?");
  else lcd.print(tempOut);
  if ((hd == 2) & (tempOut > -10)) {
    lcd.print((char)223);
  } else if (hd < 2) lcd.print((char)223);

  printNumber(hd, 3); //4
  printNumber(h, 6); //7
  printNumber(md, 10);
  printNumber(m, 13);

}

void Dwukropek() { //miganie dwukropkiem
  byte dots = digitalRead(SCK);
  if (dots != dwukropek) {
    dwukropek = dots;
    lcd.setCursor(9, 0);
    if (dots > 0) {
      lcd.print("o");
    } else lcd.print(" ");
    lcd.setCursor(9, 1);
    if (dots > 0) {
      lcd.print("o");
    } else lcd.print(" ");
  }
}

void bargraph(byte value) {
  for (byte i = 0; i < value; i++)  {
    lcd.print((char)255);
  }
  for (byte i = 0; i < (16 - value); i++)  {
    lcd.print(" ");
  }
}

void printWyj() {
  lcd.setCursor(0, 1);
  lcd.print(F("           wyj.>"));
  lcd.setCursor(0, 1);
}

