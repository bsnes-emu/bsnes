struct Cartridge {
  bool loadNormal(const char *basename);
  bool loadBsxSlotted(const char *basename, const char *slotname);
  bool loadBsx(const char *basename, const char *slotname);
  bool loadSufamiTurbo(const char *basename, const char *slotAname, const char *slotBname);
  bool loadSuperGameBoy(const char *basename, const char *slotname);
  void unload();

  string baseName, slotAName, slotBName;
  string baseXML, slotAXML, slotBXML;
  bool patchApplied;

private:
  bool loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename);
  bool loadMemory(SNES::Cartridge::NonVolatileRAM &memory);
  bool saveMemory(SNES::Cartridge::NonVolatileRAM &memory);
};

extern Cartridge cartridge;
