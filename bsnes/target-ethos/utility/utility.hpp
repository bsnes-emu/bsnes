struct Utility {
  string pathname;

  void setInterface(Emulator::Interface *emulator);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname);
  void saveMedia();

  void power();
  void reset();
  void unload();

  void setStatusText(const string &text);
};

extern Utility *utility;
