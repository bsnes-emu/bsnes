struct InputSettings : TabFrameItem {
  InputSettings(TabFrame*);
  auto activeEmulator() -> InputEmulator&;
  auto activePort() -> InputPort&;
  auto activeDevice() -> InputDevice&;
  auto reloadPorts() -> void;
  auto reloadDevices() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto inputEvent(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void;

  InputMapping* activeMapping = nullptr;

  VerticalLayout layout{this};
    HorizontalLayout selectionLayout{&layout, Size{~0, 0}};
      ComboButton emulatorList{&selectionLayout, Size{~0, 0}};
      ComboButton portList{&selectionLayout, Size{~0, 0}};
      ComboButton deviceList{&selectionLayout, Size{~0, 0}};
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
};

struct SettingsManager : Window {
  SettingsManager();

  VerticalLayout layout{this};
    TabFrame panelLayout{&layout, Size{~0, ~0}};
      InputSettings input{&panelLayout};
      AdvancedSettings advanced{&panelLayout};

  StatusBar statusBar{this};
};

extern SettingsManager* settingsManager;
