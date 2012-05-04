struct Utility {
  string pathname;

  void setInterface(Emulator::Interface *emulator);
  void loadSchema(Emulator::Interface *emulator, Emulator::Interface::Schema &schema);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname);
  void saveMedia();

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
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

private:
  string statusText;
  string statusMessage;
  time_t statusTime;
};

extern Utility *utility;
