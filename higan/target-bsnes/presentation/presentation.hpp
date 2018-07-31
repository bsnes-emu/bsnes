struct AboutWindow : Window {
  Application::Namespace tr{"AboutWindow"};
  auto create() -> void;

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{400, 85}, 0};
    TableLayout tableLayout{&layout, Size{~0, 0}};
      Label versionLabel{&tableLayout, Size{~0, 0}};
      Label versionValue{&tableLayout, Size{~0, 0}};
    //
      Label authorLabel{&tableLayout, Size{~0, 0}};
      Label authorValue{&tableLayout, Size{~0, 0}};
    //
      Label licenseLabel{&tableLayout, Size{~0, 0}};
      Label licenseValue{&tableLayout, Size{~0, 0}};
    //
      Label websiteLabel{&tableLayout, Size{~0, 0}};
      Label websiteValue{&tableLayout, Size{~0, 0}};
};

struct Presentation : Window {
  Application::Namespace tr{"Presentation"};
  auto create() -> void;

  enum : uint { RecentGames = 9, QuickStates = 9 };
  enum : uint { StatusHeight = 24 };

  auto updateStatusIcon() -> void;
  auto drawIcon(uint32_t* output, uint length, uint width, uint height) -> void;
  auto clearViewport() -> void;
  auto resizeViewport() -> void;
  auto resizeWindow() -> void;
  auto updateStatus() -> void;
  auto toggleFullscreenMode() -> void;
  auto updateDeviceMenu() -> void;
  auto updateDeviceSelections() -> void;
  auto updateSizeMenu() -> void;
  auto updateStateMenus() -> void;
  auto clearRecentGames() -> void;
  auto updateRecentGames() -> void;
  auto addRecentGame(string location) -> void;
  auto updateShaders() -> void;

  MenuBar menuBar{this};
    Menu systemMenu{&menuBar};
      MenuItem loadGame{&systemMenu};
      Menu loadRecentGame{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuItem unloadGame{&systemMenu};
      MenuSeparator portSeparator{&systemMenu};
      Menu controllerPort1{&systemMenu};
      Menu controllerPort2{&systemMenu};
      Menu expansionPort{&systemMenu};
      MenuSeparator quitSeparator{&systemMenu};
      MenuItem quit{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu sizeMenu{&settingsMenu};
      Group sizeGroup;
      Menu outputMenu{&settingsMenu};
        MenuRadioItem centerViewport{&outputMenu};
        MenuRadioItem scaleViewport{&outputMenu};
        MenuRadioItem stretchViewport{&outputMenu};
        Group outputGroup{&centerViewport, &scaleViewport, &stretchViewport};
        MenuSeparator outputSeparator{&outputMenu};
        MenuCheckItem aspectCorrection{&outputMenu};
        MenuCheckItem overscanCropping{&outputMenu};
        MenuCheckItem blurEmulation{&outputMenu};
      Menu shaderMenu{&settingsMenu};
      MenuSeparator settingsSeparatorA{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuCheckItem showStatusBar{&settingsMenu};
      MenuSeparator settingsSeparatorB{&settingsMenu};
      MenuItem videoSettings{&settingsMenu};
      MenuItem audioSettings{&settingsMenu};
      MenuItem inputSettings{&settingsMenu};
      MenuItem hotkeySettings{&settingsMenu};
      MenuItem pathSettings{&settingsMenu};
      MenuItem emulatorSettings{&settingsMenu};
      MenuItem driverSettings{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveState{&toolsMenu};
      Menu loadState{&toolsMenu};
      MenuSeparator toolsSeparatorA{&toolsMenu};
      Menu speedMenu{&toolsMenu};
        MenuRadioItem speedSlowest{&speedMenu};
        MenuRadioItem speedSlow{&speedMenu};
        MenuRadioItem speedNormal{&speedMenu};
        MenuRadioItem speedFast{&speedMenu};
        MenuRadioItem speedFastest{&speedMenu};
        Group speedGroup{&speedSlowest, &speedSlow, &speedNormal, &speedFast, &speedFastest};
      MenuCheckItem pauseEmulation{&toolsMenu};
      MenuItem frameAdvance{&toolsMenu};
      MenuItem captureScreenshot{&toolsMenu};
      MenuSeparator toolsSeparatorB{&toolsMenu};
      MenuItem cheatEditor{&toolsMenu};
      MenuItem stateManager{&toolsMenu};
      MenuItem manifestViewer{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuSeparator helpSeparator{&helpMenu};
      MenuItem about{&helpMenu};

  VerticalLayout layout{this};
    HorizontalLayout viewportLayout{&layout, Size{~0, ~0}, 0};
      Viewport viewport{&viewportLayout, Size{~0, ~0}, 0};
    HorizontalLayout statusLayout{&layout, Size{~0, StatusHeight}, 0};
      Label spacerIcon{&statusLayout, Size{8, ~0}, 0};
      Canvas statusIcon{&statusLayout, Size{16, ~0}, 0};
      Label spacerLeft{&statusLayout, Size{4, ~0}, 0};
      Label statusLeft{&statusLayout, Size{~0, ~0}, 0};
      Label statusRight{&statusLayout, Size{100, ~0}, 0};
      Label spacerRight{&statusLayout, Size{8, ~0}, 0};
};

extern AboutWindow aboutWindow;
extern Presentation presentation;
