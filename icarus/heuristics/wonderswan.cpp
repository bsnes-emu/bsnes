struct WonderSwanCartridge {
  WonderSwanCartridge(uint8_t* data, uint size);

  string manifest;

//private:
  struct Information {
  } information;
};

WonderSwanCartridge::WonderSwanCartridge(uint8_t* data, uint size) {
  if(size < 0x10000) return;

  manifest.append("board\n");
  manifest.append("  rom name=program.rom size=0x", hex(size), "\n");
}
