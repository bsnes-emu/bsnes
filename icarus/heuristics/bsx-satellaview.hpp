struct BsxSatellaviewCartridge {
  BsxSatellaviewCartridge(const uint8_t* data, unsigned size);

  string markup;
};

BsxSatellaviewCartridge::BsxSatellaviewCartridge(const uint8_t* data, unsigned size) {
  markup.append("cartridge\n");
  markup.append("  rom name=program.rom size=0x", hex(size), " type=FlashROM\n");
}
