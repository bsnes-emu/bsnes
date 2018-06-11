struct Settings : Markup::Node {
  Settings();
  auto save() -> void;
};

struct VideoSettings : TabFrameItem {
  VideoSettings(TabFrame*);
};

struct AudioSettings : TabFrameItem {
  AudioSettings(TabFrame*);
};

struct InputSettings : TabFrameItem {
  InputSettings(TabFrame*);
  auto updateControls() -> void;
  auto activePort() -> InputPort&;
  auto activeDevice() -> InputDevice&;
  auto reloadPorts() -> void;
  auto reloadDevices() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto assignMouseInput(uint id) -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput = false) -> void;

public:
  maybe<InputMapping&> activeMapping;
  Timer timer;

  VerticalLayout layout{this};
    HorizontalLayout defocusLayout{&layout, Size{~0, 0}};
      Label defocusLabel{&defocusLayout, Size{0, 0}};
      RadioLabel pauseEmulation{&defocusLayout, Size{0, 0}};
      RadioLabel blockInput{&defocusLayout, Size{0, 0}};
      RadioLabel allowInput{&defocusLayout, Size{0, 0}};
      Group defocusGroup{&pauseEmulation, &blockInput, &allowInput};
    HorizontalLayout selectionLayout{&layout, Size{~0, 0}};
      Label portLabel{&selectionLayout, Size{0, 0}};
      ComboButton portList{&selectionLayout, Size{~0, 0}};
      Label deviceLabel{&selectionLayout, Size{0, 0}};
      ComboButton deviceList{&selectionLayout, Size{~0, 0}};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button assignMouse1{&controlLayout, Size{100, 0}};
      Button assignMouse2{&controlLayout, Size{100, 0}};
      Button assignMouse3{&controlLayout, Size{100, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button assignButton{&controlLayout, Size{80, 0}};
      Button clearButton{&controlLayout, Size{80, 0}};
};

struct HotkeySettings : TabFrameItem {
  HotkeySettings(TabFrame*);
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;

public:
  maybe<InputMapping&> activeMapping;
  Timer timer;

  VerticalLayout layout{this};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button assignButton{&controlLayout, Size{80, 0}};
      Button clearButton{&controlLayout, Size{80, 0}};
};

struct PathSettings : TabFrameItem {
  PathSettings(TabFrame*);
  auto refreshPaths() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout gamesLayout{&layout, Size{~0, 0}};
      Label gamesLabel{&gamesLayout, Size{55, 0}};
      LineEdit gamesPath{&gamesLayout, Size{~0, 0}};
      Button gamesAssign{&gamesLayout, Size{80, 0}};
      Button gamesReset{&gamesLayout, Size{80, 0}};
    HorizontalLayout patchesLayout{&layout, Size{~0, 0}};
      Label patchesLabel{&patchesLayout, Size{55, 0}};
      LineEdit patchesPath{&patchesLayout, Size{~0, 0}};
      Button patchesAssign{&patchesLayout, Size{80, 0}};
      Button patchesReset{&patchesLayout, Size{80, 0}};
    HorizontalLayout savesLayout{&layout, Size{~0, 0}};
      Label savesLabel{&savesLayout, Size{55, 0}};
      LineEdit savesPath{&savesLayout, Size{~0, 0}};
      Button savesAssign{&savesLayout, Size{80, 0}};
      Button savesReset{&savesLayout, Size{80, 0}};
    HorizontalLayout cheatsLayout{&layout, Size{~0, 0}};
      Label cheatsLabel{&cheatsLayout, Size{55, 0}};
      LineEdit cheatsPath{&cheatsLayout, Size{~0, 0}};
      Button cheatsAssign{&cheatsLayout, Size{80, 0}};
      Button cheatsReset{&cheatsLayout, Size{80, 0}};
    HorizontalLayout statesLayout{&layout, Size{~0, 0}};
      Label statesLabel{&statesLayout, Size{55, 0}};
      LineEdit statesPath{&statesLayout, Size{~0, 0}};
      Button statesAssign{&statesLayout, Size{80, 0}};
      Button statesReset{&statesLayout, Size{80, 0}};
};

struct AdvancedSettings : TabFrameItem {
  AdvancedSettings(TabFrame*);

public:
  VerticalLayout layout{this};
    Label driversLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout driverLayout{&layout, Size{~0, 0}};
    Label videoDriverLabel{&driverLayout, Size{0, 0}};
    ComboButton videoDriverOption{&driverLayout, Size{~0, 0}};
    Label audioDriverLabel{&driverLayout, Size{0, 0}};
    ComboButton audioDriverOption{&driverLayout, Size{~0, 0}};
    Label inputDriverLabel{&driverLayout, Size{0, 0}};
    ComboButton inputDriverOption{&driverLayout, Size{~0, 0}};
    Label emulatorLabel{&layout, Size{~0, 0}, 2};
    CheckLabel fastPPUOption{&layout, Size{~0, 0}};
    CheckLabel fastDSPOption{&layout, Size{~0, 0}};
};

struct SettingsWindow : Window {
  SettingsWindow();
  auto setVisible(bool visible = true) -> SettingsWindow&;
  auto show(uint index) -> void;

public:
  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      VideoSettings video{&panel};
      AudioSettings audio{&panel};
      InputSettings input{&panel};
      HotkeySettings hotkeys{&panel};
      PathSettings paths{&panel};
      AdvancedSettings advanced{&panel};
  StatusBar statusBar{this};
};

extern Settings settings;
extern unique_pointer<SettingsWindow> settingsWindow;
