struct SufamiTurboCartridge {
  SufamiTurboCartridge(const uint8_t* data, unsigned size);

  string markup;
};

SufamiTurboCartridge::SufamiTurboCartridge(const uint8_t* data, unsigned size) {
  if(size < 0x20000) return;  //too small to be a valid game?
  if(memcmp(data, "BANDAI SFC-ADX", 14)) return;  //missing required header?
  unsigned romsize = data[0x36] * 0x20000;  //128KB
  unsigned ramsize = data[0x37] *   0x800;    //2KB
  bool linkable = data[0x35] != 0x00;  //TODO: unconfirmed

  markup.append("cartridge", linkable ? " linkable" : "", "\n");
  markup.append("  rom name=program.rom size=0x", hex(romsize), "\n");
  if(ramsize)
  markup.append("  ram name=save.ram size=0x", hex(ramsize), "\n");
}
