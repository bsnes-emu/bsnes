auto System::portRead(uint16 addr) -> uint8 {
  //DISP_MODE
  if(addr == 0x0060) return (
    r.unknown << 0
  | r.format  << 5
  | r.color   << 6
  | r.depth   << 7
  );

  //IEEP_DATA
  if(addr == 0x00ba) return eeprom.read(EEPROM::DataLo);
  if(addr == 0x00bb) return eeprom.read(EEPROM::DataHi);

  //IEEP_ADDR
  if(addr == 0x00bc) return eeprom.read(EEPROM::AddressLo);
  if(addr == 0x00bd) return eeprom.read(EEPROM::AddressHi);

  //IEEP_CMD
  if(addr == 0x00be) return eeprom.read(EEPROM::Status);
}

auto System::portWrite(uint16 addr, uint8 data) -> void {
  //DISP_MODE
  if(addr == 0x0060) {
    r.unknown = data.bits(0,4) & 0b01011;
    r.format  = data.bit (5);
    r.color   = data.bit (6);
    r.depth   = data.bit (7);
    return;
  }

  //IEEP_DATA
  if(addr == 0x00ba) return eeprom.write(EEPROM::DataLo, data);
  if(addr == 0x00bb) return eeprom.write(EEPROM::DataHi, data);

  //IEEP_ADDR
  if(addr == 0x00bc) return eeprom.write(EEPROM::AddressLo, data);
  if(addr == 0x00bd) return eeprom.write(EEPROM::AddressHi, data);

  //IEEP_CMD
  if(addr == 0x00be) return eeprom.write(EEPROM::Command, data);
}
