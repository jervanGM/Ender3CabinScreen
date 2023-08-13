void EEPROM_read(){
  auto_mode = (bool)EEPROM.read(MODE_CTRL_ADDR);
  cabin.colorSelection = (uint8_t)EEPROM.read(COLOR_CAB_ADDR);
  reading = (int)EEPROM.read(FAN_CTRL_ADDR);
  cabin.fan_cabin = (int)EEPROM.read(FAN_CAB_ADDR);
  cabin.brightness = (uint8_t)EEPROM.read(BRIGH_CAB_ADDR);
}

void EEPROM_write(){
  EEPROM.write(MODE_CTRL_ADDR, byte(auto_mode));
  EEPROM.write(COLOR_CAB_ADDR, byte(cabin.colorSelection));
  EEPROM.write(FAN_CTRL_ADDR, byte(reading));
  EEPROM.write(FAN_CAB_ADDR, byte(cabin.fan_cabin));
  EEPROM.write(BRIGH_CAB_ADDR, byte(cabin.brightness));

  EEPROM.commit();
}