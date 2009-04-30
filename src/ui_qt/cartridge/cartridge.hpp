class Cartridge {
public:
  SNES::Cartridge::Mode mode;
  string name;
  string cartName, slotName[2];

  SNES::Cartridge::Type detectImageType(const char *filename);
  bool loadNormal(const char *base);
  bool loadBsxSlotted(const char *base, const char *slot);
  bool loadBsx(const char *base, const char *slot);
  bool loadSufamiTurbo(const char *base, const char *slotA, const char *slotB);
  bool loadSuperGameboy(const char *base, const char *slot);
  void unload();

  static string filepath(const char*, const char*);
  static string basename(const char*);
  static string basepath(const char*);

private:
  bool loadFile(const char *name, uint8_t *&data, unsigned &size, bool compression);
  void loadMemory(SNES::MappedRAM&, const char*, const char*);
  void saveMemory(SNES::MappedRAM&, const char*, const char*);
  void loadCheats(const char *filename);
  void saveCheats(const char *filename);
} cartridge;
