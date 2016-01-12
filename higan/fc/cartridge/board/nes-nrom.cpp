//NES-NROM-128
//NES-NROM-256

struct NES_NROM : Board {
  NES_NROM(Markup::Node& document) : Board(document) {
    settings.mirror = document["board/mirror/mode"].text() == "vertical" ? 1 : 0;
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr & 0x8000) return prgrom.read(addr);
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) {
      if(settings.mirror == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
      return ppu.ciram_read(addr & 0x07ff);
    }
    if(chrram.size) return chrram.read(addr);
    return chrrom.read(addr);
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) {
      if(settings.mirror == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
      return ppu.ciram_write(addr & 0x07ff, data);
    }
    if(chrram.size) return chrram.write(addr, data);
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
  }

  struct Settings {
    bool mirror;  //0 = horizontal, 1 = vertical
  } settings;
};
