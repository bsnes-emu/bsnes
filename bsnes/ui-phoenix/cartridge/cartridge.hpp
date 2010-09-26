struct Cartridge {
  bool loadNormal(const char *filename);
  void unload();

  string baseName, slotAName, slotBName;
  string baseXML, slotAXML, slotBXML;

private:
  bool loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename);
  bool loadMemory(SNES::MappedRAM &memory, string filename, const char *extension);
  bool saveMemory(SNES::MappedRAM &memory, string filename, const char *extension);
};

extern Cartridge cartridge;
