struct Utility {
  string pathname;

  void setInterface(Emulator::Interface *emulator);
  void loadSchema(Emulator::Interface *emulator, Emulator::Interface::Schema &schema);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname);
  void saveMedia();

  void power();
  void reset();
  void unload();

  void resize(bool resizeWindow = false);
  void toggleFullScreen();
  void setStatusText(const string &text);
};

extern Utility *utility;
