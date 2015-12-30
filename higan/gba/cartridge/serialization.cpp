void Cartridge::serialize(serializer& s) {
  mrom.serialize(s);
  if(hasSRAM) sram.serialize(s);
  if(hasEEPROM) eeprom.serialize(s);
  if(hasFLASH) flash.serialize(s);
}
