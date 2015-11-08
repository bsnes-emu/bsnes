void Cartridge::serialize(serializer& s) {
  if(has_sram) sram.serialize(s);
  if(has_eeprom) eeprom.serialize(s);
  if(has_flash) flash.serialize(s);
}
