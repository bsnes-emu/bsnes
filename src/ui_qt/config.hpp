class Configuration : public configuration {
public:
  struct System {
    string video, audio, input;
    bool crashedOnLastRun;
    unsigned speed;
    bool autoSaveMemory;
    bool autoHideMenus;
  } system;

  struct File {
    bool autodetect_type;
    bool bypass_patch_crc32;
  } file;

  struct DiskBrowser {
    bool showPanel;
  } diskBrowser;

  struct Path {
    string base;     //binary path
    string user;     //user profile path (bsnes.cfg, ...)
    string startup;  //startup path
    string rom, save, state, patch, cheat, data;
    string bsx, st, sgb;
    string fragmentShader, vertexShader;

    struct Current {
      string folder, shader, cartridge, bsx, st, sgb;
      unsigned filter;  //current active filter for "Load Cartridge"
    } current;
  } path;

  struct Video {
    bool isFullscreen;
    bool synchronize;
    signed contrastAdjust, brightnessAdjust, gammaAdjust, scanlineAdjust;
    bool enableGammaRamp;
    double ntscAspectRatio, palAspectRatio;

    struct Context {
      bool correctAspectRatio;
      unsigned multiplier, region;
      unsigned hwFilter, swFilter;
    } *context, windowed, fullscreen;
  } video;

  struct Audio {
    bool synchronize;
    bool mute;
    unsigned volume, latency, outputFrequency, inputFrequency;
  } audio;

  struct Input {
    unsigned port1;
    unsigned port2;
    enum policy_t { FocusPolicyPauseEmulation, FocusPolicyIgnoreInput, FocusPolicyAllowInput };
    unsigned focusPolicy;
    bool allowInvalidInput;
  } input;

  struct Geometry {
    string mainWindow;
    string loaderWindow;
    string htmlViewerWindow;
    string aboutWindow;
    string diskBrowser;
    string folderCreator;
    string settingsWindow;
    string toolsWindow;
    string debugger;
    string breakpointEditor;
    string memoryEditor;
    string vramViewer;
  } geometry;

  bool load(const char *filename);
  Configuration();
};

Configuration &config();
