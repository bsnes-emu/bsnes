struct Settings : Markup::Node {
  using string = nall::string;

  auto load() -> void;
  auto save() -> void;
  auto process(bool load) -> void;

  string location;

  struct Video {
    string driver;
    bool exclusive = false;
    bool blocking = false;
    bool flush = false;
    string monitor = "Primary";
    string format = "Default";
    string shader = "Blur";

    uint luminance = 100;
    uint saturation = 100;
    uint gamma = 150;
    bool dimming = true;
    bool snow = false;

    string output = "Scale";
    uint multiplier = 2;
    bool aspectCorrection = true;
    bool overscan = false;
    bool blur = false;
    string filter = "None";
  } video;

  struct Audio {
    string driver;
    bool exclusive = false;
    string device;
    bool blocking = true;
    bool dynamic = false;
    uint frequency = 48000;
    uint latency = 0;

    bool mute = false;
    bool muteUnfocused = false;
    int skew = 0;
    uint volume = 100;
    uint balance = 50;
  } audio;

  struct Input {
    string driver;
    uint frequency = 5;
    string defocus = "Pause";
    struct Turbo {
      uint frequency = 4;
    } turbo;
    struct Hotkey {
      string logic = "or";
    } hotkey;
  } input;

  struct Path {
    string games;
    string patches;
    string saves;
    string cheats;
    string states;
    string screenshots;
    struct Recent {
      string superFamicom;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } recent;
  } path;

  struct FastForward {
    uint frameSkip = 9;
    uint limiter = 0;
    bool mute = false;
  } fastForward;

  struct Rewind {
    uint frequency = 0;
    uint length = 80;
    bool mute = false;
  } rewind;

  struct Emulator {
    bool warnOnUnverifiedGames = false;
    struct AutoSaveMemory {
      bool enable = true;
      uint interval = 30;
    } autoSaveMemory;
    bool autoSaveStateOnUnload = false;
    bool autoLoadStateOnLoad = false;
    struct Serialization {
      string method = "Fast";
      bool synchronize = true;
    } serialization;
    struct Hack {
      bool hotfixes = true;
      string entropy = "Low";
      struct CPU {
        uint overclock = 100;
        bool fastMath = false;
      } cpu;
      struct PPU {
        bool fast = true;
        bool deinterlace = true;
        bool noSpriteLimit = false;
        bool noVRAMBlocking = false;
        struct Mode7 {
          uint scale = 1;
          bool perspective = true;
          bool supersample = false;
          bool mosaic = true;
        } mode7;
      } ppu;
      struct DSP {
        bool fast = true;
        bool cubic = false;
        bool echoShadow = false;
      } dsp;
      struct Coprocessor {
        bool delayedSync = true;
        bool preferHLE = false;
      } coprocessor;
      struct SA1 {
        uint overclock = 100;
      } sa1;
      struct SuperFX {
        uint overclock = 100;
      } superfx;
    } hack;
    struct Cheats {
      bool enable = true;
    } cheats;
  } emulator;

  struct General {
    bool statusBar = true;
    bool screenSaver = false;
    bool toolTips = true;
    bool crashed = false;
    bool betaWarning = true;
  } general;
};

struct VideoSettings : VerticalLayout {
  auto create() -> void;

private:
  Label colorAdjustmentLabel{this, Size{~0, 0}, 2};
  TableLayout colorLayout{this, Size{~0, 0}};
    Label luminanceLabel{&colorLayout, Size{0, 0}};
    Label luminanceValue{&colorLayout, Size{50_sx, 0}};
    HorizontalSlider luminanceSlider{&colorLayout, Size{~0, 0}};
  //
    Label saturationLabel{&colorLayout, Size{0, 0}};
    Label saturationValue{&colorLayout, Size{50_sx, 0}};
    HorizontalSlider saturationSlider{&colorLayout, Size{~0, 0}};
  //
    Label gammaLabel{&colorLayout, Size{0, 0}};
    Label gammaValue{&colorLayout, Size{50_sx, 0}};
    HorizontalSlider gammaSlider{&colorLayout, Size{~0, 0}};
  //
  CheckLabel dimmingOption{this, Size{~0, 0}};
  CheckLabel snowOption{this, Size{~0, 0}};
};

struct AudioSettings : VerticalLayout {
  auto create() -> void;

private:
  Label effectsLabel{this, Size{~0, 0}, 2};
  TableLayout effectsLayout{this, Size{~0, 0}};
    Label skewLabel{&effectsLayout, Size{0, 0}};
    Label skewValue{&effectsLayout, Size{50_sx, 0}};
    HorizontalSlider skewSlider{&effectsLayout, Size{~0, 0}};
  //
    Label volumeLabel{&effectsLayout, Size{0, 0}};
    Label volumeValue{&effectsLayout, Size{50_sx, 0}};
    HorizontalSlider volumeSlider{&effectsLayout, Size{~0, 0}};
  //
    Label balanceLabel{&effectsLayout, Size{0, 0}};
    Label balanceValue{&effectsLayout, Size{50_sx, 0}};
    HorizontalSlider balanceSlider{&effectsLayout, Size{~0, 0}};
  //
  CheckLabel muteUnfocused{this, Size{~0, 0}};
};

struct InputSettings : VerticalLayout {
  auto create() -> void;
  auto updateControls() -> void;
  auto activePort() -> InputPort&;
  auto activeDevice() -> InputDevice&;
  auto reloadPorts() -> void;
  auto reloadDevices() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping(TableViewCell cell) -> void;
  auto cancelMapping() -> void;
  auto assignMouseInput(uint id) -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput = false) -> void;

  maybe<InputMapping&> activeMapping;
  uint activeBinding = 0;

public:
  Timer timer;

  HorizontalLayout inputFocusLayout{this, Size{~0, 0}};
    Label inputFocusLabel{&inputFocusLayout, Size{0, 0}};
    RadioLabel pauseEmulation{&inputFocusLayout, Size{0, 0}};
    RadioLabel blockInput{&inputFocusLayout, Size{0, 0}};
    RadioLabel allowInput{&inputFocusLayout, Size{0, 0}};
    Group inputFocusGroup{&pauseEmulation, &blockInput, &allowInput};
  Canvas separator{this, Size{~0, 1}};
  HorizontalLayout selectionLayout{this, Size{~0, 0}};
    Label portLabel{&selectionLayout, Size{0, 0}};
    ComboButton portList{&selectionLayout, Size{~0, 0}};
    Label deviceLabel{&selectionLayout, Size{0, 0}};
    ComboButton deviceList{&selectionLayout, Size{~0, 0}};
    Label turboLabel{&selectionLayout, Size{0, 0}};
    ComboButton turboList{&selectionLayout, Size{0, 0}};
  TableView mappingList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button assignMouse1{&controlLayout, Size{100_sx, 0}};
    Button assignMouse2{&controlLayout, Size{100_sx, 0}};
    Button assignMouse3{&controlLayout, Size{100_sx, 0}};
    Canvas inputSink{&controlLayout, Size{~0, ~0}};
    Button assignButton{&controlLayout, Size{80_sx, 0}};
    Button clearButton{&controlLayout, Size{80_sx, 0}};
};

struct HotkeySettings : VerticalLayout {
  auto create() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping(TableViewCell cell) -> void;
  auto cancelMapping() -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;

  maybe<InputMapping&> activeMapping;
  uint activeBinding = 0;

private:
  Timer timer;

  TableView mappingList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Label logicLabel{&controlLayout, Size{0, 0}};
    RadioLabel logicAND{&controlLayout, Size{0, 0}};
    RadioLabel logicOR{&controlLayout, Size{0, 0}};
    Group logicGroup{&logicAND, &logicOR};
    Canvas inputSink{&controlLayout, Size{~0, ~0}};
    Button assignButton{&controlLayout, Size{80_sx, 0}};
    Button clearButton{&controlLayout, Size{80_sx, 0}};
};

struct PathSettings : VerticalLayout {
  auto create() -> void;
  auto refreshPaths() -> void;

public:
  TableLayout layout{this, Size{~0, ~0}};
    Label gamesLabel{&layout, Size{0, 0}};
    LineEdit gamesPath{&layout, Size{~0, 0}};
    Button gamesAssign{&layout, Size{80_sx, 0}};
    Button gamesReset{&layout, Size{80_sx, 0}};
  //
    Label patchesLabel{&layout, Size{0, 0}};
    LineEdit patchesPath{&layout, Size{~0, 0}};
    Button patchesAssign{&layout, Size{80_sx, 0}};
    Button patchesReset{&layout, Size{80_sx, 0}};
  //
    Label savesLabel{&layout, Size{0, 0}};
    LineEdit savesPath{&layout, Size{~0, 0}};
    Button savesAssign{&layout, Size{80_sx, 0}};
    Button savesReset{&layout, Size{80_sx, 0}};
  //
    Label cheatsLabel{&layout, Size{0, 0}};
    LineEdit cheatsPath{&layout, Size{~0, 0}};
    Button cheatsAssign{&layout, Size{80_sx, 0}};
    Button cheatsReset{&layout, Size{80_sx, 0}};
  //
    Label statesLabel{&layout, Size{0, 0}};
    LineEdit statesPath{&layout, Size{~0, 0}};
    Button statesAssign{&layout, Size{80_sx, 0}};
    Button statesReset{&layout, Size{80_sx, 0}};
  //
    Label screenshotsLabel{&layout, Size{0, 0}};
    LineEdit screenshotsPath{&layout, Size{~0, 0}};
    Button screenshotsAssign{&layout, Size{80_sx, 0}};
    Button screenshotsReset{&layout, Size{80_sx, 0}};
};

struct EmulatorSettings : VerticalLayout {
  auto create() -> void;

public:
  Label optionsLabel{this, Size{~0, 0}, 2};
  CheckLabel warnOnUnverifiedGames{this, Size{~0, 0}};
  CheckLabel autoSaveMemory{this, Size{~0, 0}};
  HorizontalLayout autoStateLayout{this, Size{~0, 0}};
    CheckLabel autoSaveStateOnUnload{&autoStateLayout, Size{0, 0}};
    CheckLabel autoLoadStateOnLoad{&autoStateLayout, Size{0, 0}};
  Canvas optionsSpacer{this, Size{~0, 1}};
  //
  Label fastForwardLabel{this, Size{~0, 0}, 2};
  HorizontalLayout fastForwardLayout{this, Size{~0, 0}};
    Label frameSkipLabel{&fastForwardLayout, Size{0, 0}};
    ComboButton frameSkipAmount{&fastForwardLayout, Size{0, 0}};
    Label limiterLabel{&fastForwardLayout, Size{0, 0}};
    ComboButton limiterAmount{&fastForwardLayout, Size{0, 0}};
  CheckLabel fastForwardMute{this, Size{0, 0}};
  Canvas fastForwardSpacer{this, Size{~0, 1}};
  //
  Label rewindLabel{this, Size{~0, 0}, 2};
  HorizontalLayout rewindLayout{this, Size{~0, 0}};
    Label rewindFrequencyLabel{&rewindLayout, Size{0, 0}};
    ComboButton rewindFrequencyOption{&rewindLayout, Size{0, 0}};
    Label rewindLengthLabel{&rewindLayout, Size{0, 0}};
    ComboButton rewindLengthOption{&rewindLayout, Size{0, 0}};
  CheckLabel rewindMute{this, Size{0, 0}};
};

struct EnhancementSettings : VerticalLayout {
  auto create() -> void;

public:
  Label overclockingLabel{this, Size{~0, 0}, 2};
  TableLayout overclockingLayout{this, Size{~0, 0}};
    Label cpuLabel{&overclockingLayout, Size{0, 0}};
    Label cpuValue{&overclockingLayout, Size{50_sx, 0}};
    HorizontalSlider cpuClock{&overclockingLayout, Size{~0, 0}};
  Canvas overclockingSpacer{this, Size{~0, 1}};
  //
    Label sa1Label{&overclockingLayout, Size{0, 0}};
    Label sa1Value{&overclockingLayout, Size{50_sx, 0}};
    HorizontalSlider sa1Clock{&overclockingLayout, Size{~0, 0}};
  //
    Label sfxLabel{&overclockingLayout, Size{0, 0}};
    Label sfxValue{&overclockingLayout, Size{50_sx, 0}};
    HorizontalSlider sfxClock{&overclockingLayout, Size{~0, 0}};
  //
  Label ppuLabel{this, Size{~0, 0}, 2};
  HorizontalLayout ppuLayout{this, Size{~0, 0}};
    CheckLabel fastPPU{&ppuLayout, Size{0, 0}};
    CheckLabel deinterlace{&ppuLayout, Size{0, 0}};
    CheckLabel noSpriteLimit{&ppuLayout, Size{0, 0}};
  //
  Label mode7Label{this, Size{~0, 0}, 2};
  HorizontalLayout mode7Layout{this, Size{~0, 0}};
    Label mode7ScaleLabel{&mode7Layout, Size{0, 0}};
    ComboButton mode7Scale{&mode7Layout, Size{0, 0}};
    CheckLabel mode7Perspective{&mode7Layout, Size{0, 0}};
    CheckLabel mode7Supersample{&mode7Layout, Size{0, 0}};
    CheckLabel mode7Mosaic{&mode7Layout, Size{0, 0}};
  //
  Label dspLabel{this, Size{~0, 0}, 2};
  HorizontalLayout dspLayout{this, Size{~0, 0}};
    CheckLabel fastDSP{&dspLayout, Size{0, 0}};
    CheckLabel cubicInterpolation{&dspLayout, Size{0, 0}};
  //
  Label coprocessorLabel{this, Size{~0, 0}, 2};
  HorizontalLayout coprocessorLayout{this, Size{~0, 0}};
    CheckLabel coprocessorDelayedSyncOption{&coprocessorLayout, Size{0, 0}};
    CheckLabel coprocessorPreferHLEOption{&coprocessorLayout, Size{0, 0}};
  Canvas coprocessorSpacer{this, Size{~0, 1}};
  //
  Label gameLabel{this, Size{~0, 0}, 2};
  CheckLabel hotfixes{this, Size{0, 0}};
  //
  Widget spacer{this, Size{~0, ~0}};
  Label note{this, Size{~0, 0}};
};

struct CompatibilitySettings : VerticalLayout {
  auto create() -> void;

private:
  Label entropyLabel{this, Size{~0, 0}, 2};
  HorizontalLayout entropyLayout{this, Size{~0, 0}};
    RadioLabel entropyNone{&entropyLayout, Size{0, 0}};
    RadioLabel entropyLow{&entropyLayout, Size{0, 0}};
    RadioLabel entropyHigh{&entropyLayout, Size{0, 0}};
    Group entropyGroup{&entropyNone, &entropyLow, &entropyHigh};
  //
  Label cpuLabel{this, Size{~0, 0}, 2};
  CheckLabel fastMath{this, Size{0, 0}};
  //
  Label ppuLabel{this, Size{~0, 0}, 2};
  CheckLabel noVRAMBlocking{this, Size{0, 0}};
  //
  Label dspLabel{this, Size{~0, 0}, 2};
  CheckLabel echoShadow{this, Size{0, 0}};
  //
  Widget spacer{this, Size{~0, ~0}};
  Label note{this, Size{~0, 0}};
};

struct DriverSettings : VerticalLayout {
  auto create() -> void;
  auto videoDriverChanged() -> void;
  auto videoDriverChange() -> void;
  auto videoMonitorChanged() -> void;
  auto videoMonitorChange() -> void;
  auto videoFormatChanged() -> void;
  auto videoFormatChange() -> void;
  auto audioDriverChanged() -> void;
  auto audioDriverChange() -> void;
  auto audioDeviceChanged() -> void;
  auto audioDeviceChange() -> void;
  auto audioFrequencyChanged() -> void;
  auto audioFrequencyChange() -> void;
  auto audioLatencyChanged() -> void;
  auto audioLatencyChange() -> void;
  auto inputDriverChanged() -> void;
  auto inputDriverChange() -> void;

public:
  Label videoLabel{this, Size{~0, 0}, 2};
  VerticalLayout videoLayout{this, Size{~0, 0}};
    HorizontalLayout videoDriverLayout{&videoLayout, Size{~0, 0}};
      Label videoDriverLabel{&videoDriverLayout, Size{0, 0}};
      ComboButton videoDriverOption{&videoDriverLayout, Size{0, 0}};
      Button videoDriverUpdate{&videoDriverLayout, Size{0, 0}};
      Label videoDriverActive{&videoDriverLayout, Size{0, 0}};
    HorizontalLayout videoPropertyLayout{&videoLayout, Size{~0, 0}};
      Label videoMonitorLabel{&videoPropertyLayout, Size{0, 0}};
      ComboButton videoMonitorOption{&videoPropertyLayout, Size{0, 0}};
      Label videoFormatLabel{&videoPropertyLayout, Size{0, 0}};
      ComboButton videoFormatOption{&videoPropertyLayout, Size{0, 0}};
  HorizontalLayout videoToggleLayout{this, Size{~0, 0}};
    CheckLabel videoExclusiveToggle{&videoToggleLayout, Size{0, 0}};
    CheckLabel videoBlockingToggle{&videoToggleLayout, Size{0, 0}};
    CheckLabel videoFlushToggle{&videoToggleLayout, Size{0, 0}};
  Canvas videoSpacer{this, Size{~0, 1}};
  //
  Label audioLabel{this, Size{~0, 0}, 2};
  VerticalLayout audioLayout{this, Size{~0, 0}};
    HorizontalLayout audioDriverLayout{&audioLayout, Size{~0, 0}};
      Label audioDriverLabel{&audioDriverLayout, Size{0, 0}};
      ComboButton audioDriverOption{&audioDriverLayout, Size{0, 0}};
      Button audioDriverUpdate{&audioDriverLayout, Size{0, 0}};
      Label audioDriverActive{&audioDriverLayout, Size{0, 0}};
    HorizontalLayout audioPropertyLayout{&audioLayout, Size{~0, 0}};
      Label audioDeviceLabel{&audioPropertyLayout, Size{0, 0}};
      ComboButton audioDeviceOption{&audioPropertyLayout, Size{0, 0}};
      Label audioFrequencyLabel{&audioPropertyLayout, Size{0, 0}};
      ComboButton audioFrequencyOption{&audioPropertyLayout, Size{0, 0}};
      Label audioLatencyLabel{&audioPropertyLayout, Size{0, 0}};
      ComboButton audioLatencyOption{&audioPropertyLayout, Size{0, 0}};
  HorizontalLayout audioToggleLayout{this, Size{~0, 0}};
    CheckLabel audioExclusiveToggle{&audioToggleLayout, Size{0, 0}};
    CheckLabel audioBlockingToggle{&audioToggleLayout, Size{0, 0}};
    CheckLabel audioDynamicToggle{&audioToggleLayout, Size{0, 0}};
  Canvas audioSpacer{this, Size{~0, 1}};
  //
  Label inputLabel{this, Size{~0, 0}, 2};
  VerticalLayout inputLayout{this, Size{~0, 0}};
    HorizontalLayout inputDriverLayout{&inputLayout, Size{~0, 0}};
      Label inputDriverLabel{&inputDriverLayout, Size{0, 0}};
      ComboButton inputDriverOption{&inputDriverLayout, Size{0, 0}};
      Button inputDriverUpdate{&inputDriverLayout, Size{0, 0}};
      Label inputDriverActive{&inputDriverLayout, Size{0, 0}};
  Canvas inputSpacer{this, Size{~0, 1}};
  //
  Widget spacer{this, Size{~0, ~0}, 0};
  Label syncModeLabel{this, Size{~0, 0}, 0};
  Label syncModeRequirements{this, Size{~0, 0}};
  HorizontalLayout modeLayout{this, Size{~0, 0}};
    Button adaptiveSyncMode{&modeLayout, Size{0, 0}};
    Button dynamicRateControlMode{&modeLayout, Size{0, 0}};
};

struct SettingsWindow : Window, Lock {
  auto create() -> void;
  auto setVisible(bool visible = true) -> SettingsWindow&;
  auto show(int index) -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout panelLayout{&layout, Size{~0, ~0}};
      ListView panelList{&panelLayout, Size{125_sx, ~0}};
      VerticalLayout panelContainer{&panelLayout, Size{~0, ~0}};
  StatusBar statusBar{this};
};

extern Settings settings;
extern VideoSettings videoSettings;
extern AudioSettings audioSettings;
extern InputSettings inputSettings;
extern HotkeySettings hotkeySettings;
extern PathSettings pathSettings;
extern EmulatorSettings emulatorSettings;
extern EnhancementSettings enhancementSettings;
extern CompatibilitySettings compatibilitySettings;
extern DriverSettings driverSettings;
namespace Instances { extern Instance<SettingsWindow> settingsWindow; }
extern SettingsWindow& settingsWindow;
