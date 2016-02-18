auto Cartridge::portRead(uint16 addr) -> uint8 {
  //BANK_ROM2
  if(addr == 0x00c0) return r.bank_rom2;

  //BANK_SRAM
  if(addr == 0x00c1) return r.bank_sram;

  //BANK_ROM0
  if(addr == 0x00c2) return r.bank_rom0;

  //BANK_ROM1
  if(addr == 0x00c3) return r.bank_rom1;

  //EEP_DATA
  if(addr == 0x00c4) return eeprom.read(EEPROM::DataLo);
  if(addr == 0x00c5) return eeprom.read(EEPROM::DataHi);

  //EEP_ADDR
  if(addr == 0x00c6) return eeprom.read(EEPROM::AddressLo);
  if(addr == 0x00c7) return eeprom.read(EEPROM::AddressHi);

  //EEP_STATUS
  if(addr == 0x00c8) return eeprom.read(EEPROM::Status);

  return 0x00;
}

auto Cartridge::portWrite(uint16 addr, uint8 data) -> void {
  //BANK_ROM2
  if(addr == 0x00c0) {
    r.bank_rom2 = data;
    return;
  }

  //BANK_SRAM
  if(addr == 0x00c1) {
    r.bank_sram = data;
    return;
  }

  //BANK_ROM0
  if(addr == 0x00c2) {
    r.bank_rom0 = data;
    return;
  }

  //BANK_ROM1
  if(addr == 0x00c3) {
    r.bank_rom1 = data;
    return;
  }

  //EEP_DATA
  if(addr == 0x00c4) return eeprom.write(EEPROM::DataLo, data);
  if(addr == 0x00c5) return eeprom.write(EEPROM::DataHi, data);

  //EEP_ADDR
  if(addr == 0x00c6) return eeprom.write(EEPROM::AddressLo, data);
  if(addr == 0x00c7) return eeprom.write(EEPROM::AddressHi, data);

  //EEP_CMD
  if(addr == 0x00c8) return eeprom.write(EEPROM::Command, data);
}
