struct Utility {
  void setInterface(Emulator::Interface *emulator);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname);
  void loadMedia(Emulator::Interface::Media &media);
  void loadMemory();
  void saveMemory();

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
  void load();
  void unload();

  void saveState(unsigned slot);
  void loadState(unsigned slot);

  void synchronizeRuby();
  void resize(bool resizeWindow = false);
  void toggleFullScreen();

  void updateStatus();
  void setStatusText(const string &text);
  void showMessage(const string &message);

  Utility();

  lstring path;
  lstring pathname;

private:
  string statusText;
  string statusMessage;
  time_t statusTime;
};

extern Utility *utility;
