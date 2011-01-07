struct Utility {
  struct Cartridge {
    string basename;
  } cartridge;

  void loadCartridge(const char *filename);
  bool saveState(unsigned slot);
  bool loadState(unsigned slot);
};

extern Utility utility;
