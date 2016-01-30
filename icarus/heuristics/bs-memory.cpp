struct BSMemoryCartridge {
  BSMemoryCartridge(const uint8* data, uint size);

  string markup;
};

BSMemoryCartridge::BSMemoryCartridge(const uint8* data, uint size) {
  markup.append("board\n");
  markup.append("  rom type=flash name=program.rom size=0x", hex(size), "\n");
}
