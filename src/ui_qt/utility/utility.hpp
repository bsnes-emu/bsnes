class Utility {
public:
  //utility.cpp
  void inputEvent(uint16_t scancode);
  void showMessage(const char *message);
  void updateSystemState();
  void acquireMouse();
  void unacquireMouse();

  void updateAvSync();
  void updateVideoMode();
  void updateColorFilter();
  void updatePixelShader();
  void updateHardwareFilter();
  void updateSoftwareFilter();
  void updateEmulationSpeed();
  void updateControllers();

  //cartridge.cpp
  struct Cartridge {
    string name;      //printable cartridge name
    string fileName;  //ideal file name for saving eg states to disk
    string baseName, slotAName, slotBName;
    bool patchApplied;
  } cartridge;

  bool loadCartridgeNormal(const char*);
  bool loadCartridgeBsxSlotted(const char*, const char*);
  bool loadCartridgeBsx(const char*, const char*);
  bool loadCartridgeSufamiTurbo(const char*, const char *, const char*);
  bool loadCartridgeSuperGameBoy(const char*, const char*);
  void saveMemory();
  void unloadCartridge();

  enum system_state_t { LoadCartridge, UnloadCartridge, PowerOn, PowerOff, PowerCycle, Reset };
  void modifySystemState(system_state_t state);

  bool loadCartridge(string&, SNES::MappedRAM&);
  bool loadMemory(const char*, const char*, SNES::MappedRAM&);
  bool saveMemory(const char*, const char*, SNES::MappedRAM&);
  void loadCheats();
  void saveCheats();

  bool saveStatesSupported();
  string filepath(const char *filename, const char *pathname);

  //window.cpp
  void updateFullscreenState();
  void constrainSize(unsigned &x, unsigned &y, unsigned max);
  void resizeMainWindow();
  void toggleSynchronizeVideo();
  void toggleSynchronizeAudio();
  void setNtscMode();
  void setPalMode();
  void toggleSmoothVideoOutput();
  void toggleAspectCorrection();
  void setScale(unsigned);
  void toggleFullscreen();
  void toggleMenubar();
  void toggleStatusbar();
};

extern Utility utility;
