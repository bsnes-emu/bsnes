struct VideoSettings : TabFrameItem {
  VideoSettings(TabFrame*);

  VerticalLayout layout{this};
    Label colorAdjustmentLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout saturationLayout{&layout, Size{~0, 0}};
      Label saturationLabel{&saturationLayout, Size{80, 0}};
      Label saturationValue{&saturationLayout, Size{50, 0}};
      HorizontalSlider saturationSlider{&saturationLayout, Size{~0, 0}};
    HorizontalLayout gammaLayout{&layout, Size{~0, 0}};
      Label gammaLabel{&gammaLayout, Size{80, 0}};
      Label gammaValue{&gammaLayout, Size{50, 0}};
      HorizontalSlider gammaSlider{&gammaLayout, Size{~0, 0}};
    HorizontalLayout luminanceLayout{&layout, Size{~0, 0}};
      Label luminanceLabel{&luminanceLayout, Size{80, 0}};
      Label luminanceValue{&luminanceLayout, Size{50, 0}};
      HorizontalSlider luminanceSlider{&luminanceLayout, Size{~0, 0}};
    Label overscanMaskLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout horizontalMaskLayout{&layout, Size{~0, 0}};
      Label horizontalMaskLabel{&horizontalMaskLayout, Size{80, 0}};
      Label horizontalMaskValue{&horizontalMaskLayout, Size{50, 0}};
      HorizontalSlider horizontalMaskSlider{&horizontalMaskLayout, Size{~0, 0}};
    HorizontalLayout verticalMaskLayout{&layout, Size{~0, 0}};
      Label verticalMaskLabel{&verticalMaskLayout, Size{80, 0}};
      Label verticalMaskValue{&verticalMaskLayout, Size{50, 0}};
      HorizontalSlider verticalMaskSlider{&verticalMaskLayout, Size{~0, 0}};
    Label windowedModeLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout windowedModeLayout{&layout, Size{~0, 0}};
      CheckLabel windowedModeAspectCorrection{&windowedModeLayout, Size{0, 0}};
      CheckLabel windowedModeIntegralScaling{&windowedModeLayout, Size{0, 0}};
      CheckLabel windowedModeAdaptive{&windowedModeLayout, Size{0, 0}};
    Label fullscreenModeLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout fullscreenModeLayout{&layout, Size{~0, 0}};
      CheckLabel fullscreenModeAspectCorrection{&fullscreenModeLayout, Size{0, 0}};
      CheckLabel fullscreenModeIntegralScaling{&fullscreenModeLayout, Size{0, 0}};
      CheckLabel fullscreenModeExclusive{&fullscreenModeLayout, Size{0, 0}};

  auto updateColor(bool initializing = false) -> void;
  auto updateViewport(bool initializing = false) -> void;
};

struct AudioSettings : TabFrameItem {
  AudioSettings(TabFrame*);

  VerticalLayout layout{this};
    Label driverLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label deviceLabel{&controlLayout, Size{0, 0}};
      ComboButton deviceList{&controlLayout, Size{~0, 0}};
      Label frequencyLabel{&controlLayout, Size{0, 0}};
      ComboButton frequencyList{&controlLayout, Size{~0, 0}};
      Label latencyLabel{&controlLayout, Size{0, 0}};
      ComboButton latencyList{&controlLayout, Size{~0, 0}};
    CheckLabel exclusiveMode{&layout, Size{~0, 0}};
    Label effectsLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout volumeLayout{&layout, Size{~0, 0}};
      Label volumeLabel{&volumeLayout, Size{80, 0}};
      Label volumeValue{&volumeLayout, Size{50, 0}};
      HorizontalSlider volumeSlider{&volumeLayout, Size{~0, 0}};
    HorizontalLayout balanceLayout{&layout, Size{~0, 0}};
      Label balanceLabel{&balanceLayout, Size{80, 0}};
      Label balanceValue{&balanceLayout, Size{50, 0}};
      HorizontalSlider balanceSlider{&balanceLayout, Size{~0, 0}};
    CheckLabel reverbEnable{&layout, Size{~0, 0}};

  auto updateDriver(bool initializing = false) -> void;
  auto updateEffects(bool initializing = false) -> void;
  auto updateDriverLists() -> void;
};

struct InputSettings : TabFrameItem {
  InputSettings(TabFrame*);

  VerticalLayout layout{this};
    HorizontalLayout focusLayout{&layout, Size{~0, 0}};
      Label focusLabel{&focusLayout, Size{0, 0}};
      CheckLabel pauseEmulation{&focusLayout, Size{0, 0}};
      CheckLabel allowInput{&focusLayout, Size{0, 0}};
    HorizontalLayout selectionLayout{&layout, Size{~0, 0}};
      ComboButton emulatorList{&selectionLayout, Size{~0, 0}};
      ComboButton portList{&selectionLayout, Size{~0, 0}};
      ComboButton deviceList{&selectionLayout, Size{~0, 0}};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button assignMouse1{&controlLayout, Size{100, 0}};
      Button assignMouse2{&controlLayout, Size{100, 0}};
      Button assignMouse3{&controlLayout, Size{100, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};

  auto updateControls() -> void;
  auto activeEmulator() -> InputEmulator&;
  auto activePort() -> InputPort&;
  auto activeDevice() -> InputDevice&;
  auto reloadPorts() -> void;
  auto reloadDevices() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto assignMouseInput(uint id) -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput = false) -> void;

  InputMapping* activeMapping = nullptr;
  Timer timer;
};

struct HotkeySettings : TabFrameItem {
  HotkeySettings(TabFrame*);

  VerticalLayout layout{this};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};

  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;

  InputMapping* activeMapping = nullptr;
  Timer timer;
};

struct AdvancedSettings : TabFrameItem {
  AdvancedSettings(TabFrame*);

  VerticalLayout layout{this};
    Label driverLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout driverLayout{&layout, Size{~0, 0}};
      Label videoLabel{&driverLayout, Size{0, 0}};
      ComboButton videoDriver{&driverLayout, Size{~0, 0}};
      Label audioLabel{&driverLayout, Size{0, 0}};
      ComboButton audioDriver{&driverLayout, Size{~0, 0}};
      Label inputLabel{&driverLayout, Size{0, 0}};
      ComboButton inputDriver{&driverLayout, Size{~0, 0}};
    Label libraryLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout libraryLayout{&layout, Size{~0, 0}};
      Label libraryPrefix{&libraryLayout, Size{0, 0}};
      LineEdit libraryLocation{&libraryLayout, Size{~0, 0}};
      Button libraryChange{&libraryLayout, Size{0, 0}};
    CheckLabel ignoreManifests{&layout, Size{~0, 0}};
};

struct SettingsManager : Window {
  SettingsManager();

  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      VideoSettings video{&panel};
      AudioSettings audio{&panel};
      InputSettings input{&panel};
      HotkeySettings hotkeys{&panel};
      AdvancedSettings advanced{&panel};
  StatusBar statusBar{this};

  auto setVisible(bool visible = true) -> SettingsManager&;
  auto show(uint setting) -> void;
};

extern unique_pointer<SettingsManager> settingsManager;
