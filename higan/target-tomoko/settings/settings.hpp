struct VideoSettings : TabFrameItem {
  VideoSettings(TabFrame*);

  VerticalLayout layout{this};
    Label overscanMaskLabel{&layout, Size{~0, 0}};
    HorizontalLayout horizontalMaskLayout{&layout, Size{~0, 0}};
      Label horizontalMaskLabel{&horizontalMaskLayout, Size{80, 0}};
      Label horizontalMaskValue{&horizontalMaskLayout, Size{80, 0}};
      HorizontalSlider horizontalMaskSlider{&horizontalMaskLayout, Size{~0, 0}};
    HorizontalLayout verticalMaskLayout{&layout, Size{~0, 0}};
      Label verticalMaskLabel{&verticalMaskLayout, Size{80, 0}};
      Label verticalMaskValue{&verticalMaskLayout, Size{80, 0}};
      HorizontalSlider verticalMaskSlider{&verticalMaskLayout, Size{~0, 0}};

  auto update() -> void;
};

struct AudioSettings : TabFrameItem {
  AudioSettings(TabFrame*);

  VerticalLayout layout{this};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label frequencyLabel{&controlLayout, Size{0, 0}};
      ComboButton frequencyCombo{&controlLayout, Size{~0, 0}};
      Label latencyLabel{&controlLayout, Size{0, 0}};
      ComboButton latencyCombo{&controlLayout, Size{~0, 0}};
      Label resamplerLabel{&controlLayout, Size{0, 0}};
      ComboButton resamplerCombo{&controlLayout, Size{~0, 0}};
    HorizontalLayout volumeLayout{&layout, Size{~0, 0}};
      Label volumeLabel{&volumeLayout, Size{80, 0}};
      Label volumeValue{&volumeLayout, Size{80, 0}};
      HorizontalSlider volumeSlider{&volumeLayout, Size{~0, 0}};
    CheckLabel exclusiveMode{&layout, Size{~0, 0}};

  auto update() -> void;
  auto updateMode() -> void;
  auto updateVolume() -> void;
};

struct InputSettings : TabFrameItem {
  InputSettings(TabFrame*);
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

  VerticalLayout layout{this};
    HorizontalLayout focusLayout{&layout, Size{~0, 0}};
      Label focusLabel{&focusLayout, Size{0, 0}};
      CheckLabel pauseEmulation{&focusLayout, Size{0, 0}};
      CheckLabel allowInput{&focusLayout, Size{0, 0}};
    HorizontalLayout selectionLayout{&layout, Size{~0, 0}};
      ComboButton emulatorList{&selectionLayout, Size{~0, 0}};
      ComboButton portList{&selectionLayout, Size{~0, 0}};
      ComboButton deviceList{&selectionLayout, Size{~0, 0}};
    ListView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button assignMouse1{&controlLayout, Size{100, 0}};
      Button assignMouse2{&controlLayout, Size{100, 0}};
      Button assignMouse3{&controlLayout, Size{100, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};
};

struct HotkeySettings : TabFrameItem {
  HotkeySettings(TabFrame*);
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;

  InputMapping* activeMapping = nullptr;
  Timer timer;

  VerticalLayout layout{this};
    ListView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};
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
  auto setVisible(bool visible = true) -> SettingsManager&;
  auto show(uint setting) -> void;

  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      VideoSettings video{&panel};
      AudioSettings audio{&panel};
      InputSettings input{&panel};
      HotkeySettings hotkeys{&panel};
      AdvancedSettings advanced{&panel};

  StatusBar statusBar{this};
};

extern unique_pointer<SettingsManager> settingsManager;
