struct Utility {
  void setInterface(Emulator::Interface* emulator);

  void loadMedia(string pathname);
  void loadMedia(Emulator::Interface* emulator, Emulator::Interface::Media& media);
  void loadMedia(Emulator::Interface* emulator, Emulator::Interface::Media& media, const string& pathname);

  void loadRequest(unsigned id, const string& name, const string& type);
  void loadRequest(unsigned id, const string& path);
  void saveRequest(unsigned id, const string& path);

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
  void load();
  void unload();

  void saveState(unsigned slot);
  void loadState(unsigned slot);

  void tracerToggle();

  void synchronizeDSP();
  void synchronizeRuby();
  void updateShader();
  void resize(bool resizeWindow = false);
  void toggleFullScreen();

  void updateStatus();
  void setStatusText(const string& text);
  void showMessage(const string& message);

  Utility();

  lstring path;
  lstring pathname;

private:
  bool tracerEnable;
  string statusText;
  string statusMessage;
  time_t statusTime;
};

extern Utility* utility;
