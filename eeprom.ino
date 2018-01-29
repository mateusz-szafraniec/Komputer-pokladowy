void saveDataToEEPROM() {
  EEPROMWritelong(passed_seconds_ADDR, passed_seconds);
  EEPROMWritelong(passed_speed_ticks_ADDR, passed_speed_ticks);
  EEPROMWriteUInt64_t((const uint16_t)passed_inj_ticks_ADDR, passed_inj_ticks);
  EEPROMWriteInt(DOTANKOWANIE_ADDR,DOTANKOWANIE);
}

void saveODOToEEPROM() {
  EEPROMWritelong(ODO_passed_seconds_ADDR, ODO_passed_seconds);
  EEPROMWritelong(ODO_passed_speed_ticks_ADDR, ODO_passed_speed_ticks);
  EEPROMWriteUInt64_t((const uint16_t)ODO_passed_inj_ticks_ADDR, ODO_passed_inj_ticks);
  EEPROMWriteInt(DOTANKOWANIE_ADDR,DOTANKOWANIE);
}

void readDataFromEEPROM() {
  passed_seconds = EEPROMReadlong(passed_seconds_ADDR);
  passed_speed_ticks = EEPROMReadlong(passed_speed_ticks_ADDR);
  passed_inj_ticks = EEPROMReadUInt64_t(passed_inj_ticks_ADDR, 0);

  ODO_passed_seconds = EEPROMReadlong(ODO_passed_seconds_ADDR);
  ODO_passed_speed_ticks = EEPROMReadlong(ODO_passed_speed_ticks_ADDR);
  ODO_passed_inj_ticks = EEPROMReadUInt64_t(ODO_passed_inj_ticks_ADDR, 0);

#ifdef UART_SPEED
  Serial.println(passed_seconds);
  Serial.println(passed_speed_ticks);
  Serial.println((uint32_t)passed_inj_ticks);
#endif

  if (passed_seconds == 0xFFFFFFFF) passed_seconds = 0;
  if (passed_speed_ticks == 0xFFFFFFFF) passed_speed_ticks = 0;
  if (passed_inj_ticks == 0xFFFFFFFFFFFFFFFF) passed_inj_ticks = 0;

  if (ODO_passed_seconds == 0xFFFFFFFF) ODO_passed_seconds = 0;
  if (ODO_passed_speed_ticks == 0xFFFFFFFF) ODO_passed_speed_ticks = 0;
  if (ODO_passed_inj_ticks == 0xFFFFFFFFFFFFFFFF) ODO_passed_inj_ticks = 0;
}

//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
unsigned long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

/// Fetch an int64 value from eeprom
uint64_t EEPROMReadUInt64_t(const uint16_t location, const uint64_t default_value) {
  uint64_t *ret;
  uint8_t data[8];
  eeprom_read_block(data, (const uint8_t*)location, 8);
  if (data[0] == 0xff && data[1] == 0xff && data[2] == 0xff && data[3] == 0xff &&
      data[4] == 0xff && data[5] == 0xff && data[6] == 0xff && data[7] == 0xff)
    return default_value;
  ret = (uint64_t *)&data[0];
  return *ret;
}


/// Write an int64 value to eeprom
void EEPROMWriteUInt64_t(const uint16_t location, const uint64_t value) {
  void *data;
  data = (void *)&value;
  eeprom_write_block(data, (void*)location, 8);
}

