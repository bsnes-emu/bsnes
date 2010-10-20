class Configuration : public configuration {
public:
  struct System {
    string profile;
    string video, audio, input;
    bool crashedOnLastRun;
    unsigned speed;
    unsigned speedSlowest;
    unsigned speedSlow;
    unsigned speedNormal;
    unsigned speedFast;
    unsigned speedFastest;
    bool autoSaveMemory;
    bool rewindEnabled;
  } system;

  struct File {
    bool applyPatches;
    bool bypassPatchCrc32;
  } file;

  struct DiskBrowser {
    bool useCommonDialogs;
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
      string folder, movie, shader, cartridge;
      unsigned filter;  //current active filter for "Load Cartridge"
    } current;
  } path;

  struct Video {
    bool isFullscreen;
    bool synchronize;

    bool autoHideFullscreenMenu;

    signed contrastAdjust, brightnessAdjust, gammaAdjust, scanlineAdjust;
    bool enableGammaRamp;
    double ntscAspectRatio, palAspectRatio;

    unsigned cropLeft;
    unsigned cropTop;
    unsigned cropRight;
    unsigned cropBottom;

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
    bool modifierEnable;
  } input;

  struct Debugger {
    bool cacheUsageToDisk;
  } debugger;

  struct Geometry {
    string mainWindow;
    string loaderWindow;
    string stateSelectWindow;
    string htmlViewerWindow;
    string aboutWindow;
    string fileBrowser;
    string folderCreator;
    string settingsWindow;
    string toolsWindow;
    string cheatImportWindow;

    string debugger;
    string disassembler;
    string breakpointEditor;
    string memoryEditor;
    string propertiesViewer;
    string layerToggle;
    string vramViewer;
    string oamViewer;
    string cgramViewer;
    string debuggerOptions;
  } geometry;

  bool load(const char *filename);
  Configuration();
};

Configuration &config();
