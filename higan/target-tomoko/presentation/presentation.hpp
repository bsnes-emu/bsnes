struct AboutWindow : Window {
  AboutWindow();

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{399, 95}, 15};
    HorizontalLayout informationLayout{&layout, Size{~0, 0}};
      Label informationLeft{&informationLayout, Size{~0, 0}, 3};
      Label informationRight{&informationLayout, Size{~0, 0}};
};

struct Presentation : Window {
  Presentation();
  auto updateEmulator() -> void;
  auto clearViewport() -> void;
  auto resizeViewport(bool resizeWindow = true) -> void;
  auto toggleFullScreen() -> void;
  auto loadShaders() -> void;

  MenuBar menuBar{this};
    Menu libraryMenu{&menuBar};
    Menu systemMenu{&menuBar};
      Menu inputPort1{&systemMenu};
      Menu inputPort2{&systemMenu};
      Menu inputPort3{&systemMenu};
      MenuSeparator systemMenuSeparatorPorts{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuItem powerSystem{&systemMenu};
      MenuItem unloadSystem{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu videoScaleMenu{&settingsMenu};
        MenuItem videoScaleSmall{&videoScaleMenu};
        MenuItem videoScaleMedium{&videoScaleMenu};
        MenuItem videoScaleLarge{&videoScaleMenu};
      Menu videoEmulationMenu{&settingsMenu};
        MenuCheckItem blurEmulation{&videoEmulationMenu};
        MenuCheckItem colorEmulation{&videoEmulationMenu};
        MenuCheckItem scanlineEmulation{&videoEmulationMenu};
      Menu videoShaderMenu{&settingsMenu};
        MenuRadioItem videoShaderNone{&videoShaderMenu};
        MenuRadioItem videoShaderBlur{&videoShaderMenu};
        Group videoShaders{&videoShaderNone, &videoShaderBlur};
      MenuSeparator videoSettingsSeparator{&settingsMenu};
      MenuCheckItem synchronizeVideo{&settingsMenu};
      MenuCheckItem synchronizeAudio{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuCheckItem showStatusBar{&settingsMenu};
      MenuSeparator settingsSeparator{&settingsMenu};
      MenuItem showVideoSettings{&settingsMenu};
      MenuItem showAudioSettings{&settingsMenu};
      MenuItem showInputSettings{&settingsMenu};
      MenuItem showHotkeySettings{&settingsMenu};
      MenuItem showAdvancedSettings{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveQuickStateMenu{&toolsMenu};
        MenuItem saveSlot1{&saveQuickStateMenu};
        MenuItem saveSlot2{&saveQuickStateMenu};
        MenuItem saveSlot3{&saveQuickStateMenu};
        MenuItem saveSlot4{&saveQuickStateMenu};
        MenuItem saveSlot5{&saveQuickStateMenu};
      Menu loadQuickStateMenu{&toolsMenu};
        MenuItem loadSlot1{&loadQuickStateMenu};
        MenuItem loadSlot2{&loadQuickStateMenu};
        MenuItem loadSlot3{&loadQuickStateMenu};
        MenuItem loadSlot4{&loadQuickStateMenu};
        MenuItem loadSlot5{&loadQuickStateMenu};
      MenuSeparator toolsMenuSeparator{&toolsMenu};
      MenuItem cheatEditor{&toolsMenu};
      MenuItem stateManager{&toolsMenu};
      MenuItem manifestViewer{&toolsMenu};
      MenuItem gameNotes{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuItem credits{&helpMenu};
      MenuItem about{&helpMenu};

  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};

  StatusBar statusBar{this};
};

extern unique_pointer<AboutWindow> aboutWindow;
extern unique_pointer<Presentation> presentation;
