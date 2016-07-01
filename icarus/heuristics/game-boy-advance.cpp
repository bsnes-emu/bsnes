struct GameBoyAdvanceCartridge {
  GameBoyAdvanceCartridge(const uint8_t* data, unsigned size);

  string markup;
  string identifiers;
};

GameBoyAdvanceCartridge::GameBoyAdvanceCartridge(const uint8_t* data, unsigned size) {
  struct Identifier {
    string name;
    unsigned size;
  };
  vector<Identifier> idlist;
  idlist.append({"SRAM_V",      6});
  idlist.append({"SRAM_F_V",    8});
  idlist.append({"EEPROM_V",    8});
  idlist.append({"FLASH_V",     7});
  idlist.append({"FLASH512_V", 10});
  idlist.append({"FLASH1M_V",   9});

  string_vector list;
  for(auto& id : idlist) {
    for(signed n = 0; n < size - 16; n++) {
      if(!memcmp(data + n, (const char*)id.name, id.size)) {
        const char* p = (const char*)data + n + id.size;
        if(p[0] >= '0' && p[0] <= '9'
        && p[1] >= '0' && p[1] <= '9'
        && p[2] >= '0' && p[2] <= '9'
        ) {
          char text[16];
          memcpy(text, data + n, id.size + 3);
          text[id.size + 3] = 0;
          if(!list.find(text)) list.append(text);
        }
      }
    }
  }
  identifiers = list.merge(",");

  markup.append("board\n");
  markup.append("  rom type=mrom name=program.rom size=0x", hex(size), "\n");
  if(0);
  else if(identifiers.beginsWith("SRAM_V"    )) markup.append("  ram type=sram name=save.ram size=0x8000\n");
  else if(identifiers.beginsWith("SRAM_F_V"  )) markup.append("  ram type=sram name=save.ram size=0x8000\n");
  else if(identifiers.beginsWith("EEPROM_V"  )) markup.append("  ram type=eeprom name=save.ram size=0x0\n");
  else if(identifiers.beginsWith("FLASH_V"   )) markup.append("  ram type=flash name=save.ram size=0x10000\n");
  else if(identifiers.beginsWith("FLASH512_V")) markup.append("  ram type=flash name=save.ram size=0x10000\n");
  else if(identifiers.beginsWith("FLASH1M_V" )) markup.append("  ram type=flash name=save.ram size=0x20000\n");
//if(identifiers) markup.append("  #detected: ", identifiers, "\n");
}
