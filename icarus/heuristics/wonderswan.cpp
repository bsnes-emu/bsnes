struct WonderSwanCartridge {
  WonderSwanCartridge(uint8_t* data, uint size);

  string manifest;

//private:
  struct Information {
    bool color;

    string ramType;
    uint ramSize;
  } information;
};

WonderSwanCartridge::WonderSwanCartridge(uint8_t* data, uint size) {
  if(size < 0x10000) return;

  auto metadata = data + size - 16;

  information.color = metadata[7];

  switch(metadata[11]) {
  default:   information.ramType = ""; information.ramSize = 0; break;
  case 0x01: information.ramType = "sram"; information.ramSize =   8 * 1024; break;
  case 0x02: information.ramType = "sram"; information.ramSize =  32 * 1024; break;
  case 0x03: information.ramType = "sram"; information.ramSize = 128 * 1024; break;
  case 0x04: information.ramType = "sram"; information.ramSize = 256 * 1024; break;
  case 0x05: information.ramType = "sram"; information.ramSize = 512 * 1024; break;
  case 0x10: information.ramType = "eeprom"; information.ramSize =  128; break;
  case 0x20: information.ramType = "eeprom"; information.ramSize = 2048; break;
  case 0x50: information.ramType = "eeprom"; information.ramSize = 1024; break;
  }

  manifest.append("board\n");
  manifest.append("  rom name=program.rom size=0x", hex(size), "\n");
  if(information.ramType && information.ramSize)
  manifest.append("  ram name=save.ram type=", information.ramType, " size=0x", hex(information.ramSize), "\n");
}
