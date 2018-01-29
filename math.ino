/*
 * This function return fuel consumption in l/h * 1000
 * We assume that fuel consumption is not greater than 65.536 l/h
 * Although l_inj_ticks is uint64_t, it cannot be greater than
 * xxxxxx and
 */
uint16_t calc_fuel_h(uint64_t l_inj_ticks, uint32_t l_seconds)
{
  return (uint16_t)((l_inj_ticks * INJ_FLOW * 15 * RATIO) / ((uint64_t)4096 * l_seconds));
}

uint16_t calc_fuel_100(uint16_t l_fuel_h, uint64_t l_speed_ticks, uint32_t l_seconds)
{
  return (uint16_t)(((uint64_t)l_fuel_h * SPEED_TICKS * l_seconds) / (l_speed_ticks * 36));
}

uint16_t calc_speed_m(uint64_t l_speed_ticks, uint32_t l_seconds)
{
  return (uint16_t)(l_speed_ticks * 1000 / (SPEED_TICKS * l_seconds));
}


uint32_t power(uint32_t x, uint8_t y)
{
  uint8_t i;
  uint32_t retval = 1;
  for (i = 0; i < y; i++, retval *= x);
  return retval;
}

