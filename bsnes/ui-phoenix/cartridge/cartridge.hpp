struct Cartridge {
  bool loadNormal(const char *basename);
  bool loadBsxSlotted(const char *basename, const char *slotname);
  bool loadBsx(const char *basename, const char *slotname);
  bool loadSufamiTurbo(const char *basename, const char *slotAname, const char *slotBname);
  bool loadSuperGameBoy(const char *basename, const char *slotname);
  void unload();

  string baseName, slotAName, slotBName;
  string baseXML, slotAXML, slotBXML;

private:
  bool loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename);
  bool loadMemory(SNES::MappedRAM &memory, string filename, const char *extension);
  bool saveMemory(SNES::MappedRAM &memory, string filename, const char *extension);
};

extern Cartridge cartridge;
