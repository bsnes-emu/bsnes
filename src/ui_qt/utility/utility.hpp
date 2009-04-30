class Utility {
public:
  //utility.cpp
  bool isButtonDown(uint16_t inputCode, InputObject &object);
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
  enum CartridgeType {
    AnyCartridge     = 1 << 0,
    SnesCartridge    = 1 << 1,
    GameboyCartridge = 1 << 2,
  };
  string selectCartridge(unsigned cartridgeType);
  string selectFolder(const char *title);
  void loadCartridge(const char*);
  void unloadCartridge();

  enum system_state_t { LoadCartridge, UnloadCartridge, PowerOn, PowerOff, PowerCycle, Reset };
  void modifySystemState(system_state_t state);

  //window.cpp
  void showCentered(QWidget *window);
  void updateFullscreenState();
  void constrainSize(unsigned &x, unsigned &y, unsigned max);
  void resizeMainWindow();
} utility;
