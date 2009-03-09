class Utility {
public:
  //utility.cpp
  void inputEvent(uint16_t code);
  void showMessage(const char *message);
  void updateSystemState();
  void acquireMouse();
  void unacquireMouse();

  void updateAvSync();
  void updateVideoMode();
  void updateColorFilter();
  void updateHardwareFilter();
  void updateSoftwareFilter();
  void updateEmulationSpeed();
  void updateControllers();

  //cartridge.cpp
  string selectCartridge();
  string selectFolder(const char *title);
  void loadCartridge(const char*);
  bool loadCartridgeNormal(const char*);
  bool loadCartridgeBsxSlotted(const char*, const char*);
  bool loadCartridgeBsx(const char*, const char*);
  bool loadCartridgeSufamiTurbo(const char*, const char *, const char*);
  void unloadCartridge();

  enum system_state_t { LoadCartridge, UnloadCartridge, PowerOn, PowerOff, PowerCycle, Reset };
  void modifySystemState(system_state_t state);

  //window.cpp
  void centerWindow(QWidget *window);
  void updateFullscreenState();
  void constrainSize(unsigned &x, unsigned &y, unsigned max);
  void resizeMainWindow();
} utility;
