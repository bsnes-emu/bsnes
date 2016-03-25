auto Cartridge::portRead(uint16 addr) -> uint8 {
  //BANK_ROM2
  if(addr == 0x00c0) return r.romBank2;

  //BANK_SRAM
  if(addr == 0x00c1) return r.sramBank;

  //BANK_ROM0
  if(addr == 0x00c2) return r.romBank0;

  //BANK_ROM1
  if(addr == 0x00c3) return r.romBank1;

  //EEP_DATA
  if(addr == 0x00c4) return eeprom.read(EEPROM::DataLo);
  if(addr == 0x00c5) return eeprom.read(EEPROM::DataHi);

  //EEP_ADDR
  if(addr == 0x00c6) return eeprom.read(EEPROM::AddressLo);
  if(addr == 0x00c7) return eeprom.read(EEPROM::AddressHi);

  //EEP_STATUS
  if(addr == 0x00c8) return eeprom.read(EEPROM::Status);

  //RTC_STATUS
  if(addr == 0x00ca) return rtcStatus();

  //RTC_DATA
  if(addr == 0x00cb) return rtcRead();

  return 0x00;
}

auto Cartridge::portWrite(uint16 addr, uint8 data) -> void {
  //BANK_ROM2
  if(addr == 0x00c0) r.romBank2 = data;

  //BANK_SRAM
  if(addr == 0x00c1) r.sramBank = data;

  //BANK_ROM0
  if(addr == 0x00c2) r.romBank0 = data;

  //BANK_ROM1
  if(addr == 0x00c3) r.romBank1 = data;

  //EEP_DATA
  if(addr == 0x00c4) eeprom.write(EEPROM::DataLo, data);
  if(addr == 0x00c5) eeprom.write(EEPROM::DataHi, data);

  //EEP_ADDR
  if(addr == 0x00c6) eeprom.write(EEPROM::AddressLo, data);
  if(addr == 0x00c7) eeprom.write(EEPROM::AddressHi, data);

  //EEP_CMD
  if(addr == 0x00c8) eeprom.write(EEPROM::Command, data);

  //RTC_CMD
  if(addr == 0x00ca) rtcCommand(data);

  //RTC_DATA
  if(addr == 0x00cb) rtcWrite(data);
}
