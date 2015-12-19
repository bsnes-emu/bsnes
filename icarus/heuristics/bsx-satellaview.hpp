struct BsxSatellaviewCartridge {
  BsxSatellaviewCartridge(const uint8_t* data, unsigned size);

  string markup;
};

BsxSatellaviewCartridge::BsxSatellaviewCartridge(const uint8_t* data, unsigned size) {
  markup.append("board\n");
  markup.append("  rom type=flash name=program.rom size=0x", hex(size), "\n");
}
