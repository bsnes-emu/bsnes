struct NROM : Board {

struct Settings {
  enum class Mirror : unsigned { Horizontal, Vertical } mirror;
} settings;

uint8 prg_read(unsigned addr) {
  if(addr & 0x8000) return Board::prg_read(addr);
  return cpu.mdr();
}

void prg_write(unsigned addr, uint8 data) {
}

uint8 chr_read(unsigned addr) {
  if(addr & 0x2000) {
    if(settings.mirror == Settings::Mirror::Horizontal) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_read(addr & 0x07ff);
  }
  return Board::chr_read(addr);
}

void chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) {
    if(settings.mirror == Settings::Mirror::Horizontal) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_write(addr, data);
  }
  return Board::chr_write(addr, data);
}

void configure(const string &xml) {
  xml_element document = xml_parse(xml);
  for(auto &node : document.element) {
    if(node.name == "mirror") {
      for(auto &attr : node.attribute) {
        if(attr.name == "type") {
          if(attr.content == "horizontal") settings.mirror = Settings::Mirror::Horizontal;
          if(attr.content == "vertical"  ) settings.mirror = Settings::Mirror::Vertical;
        }
      }
    }
  }
}

};

NROM nrom;
