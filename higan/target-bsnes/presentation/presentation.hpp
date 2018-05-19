struct AboutWindow : Window {
  AboutWindow();

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{400, 86}, 0};
    HorizontalLayout informationLayout{&layout, Size{~0, ~0}};
      Label informationLeft{&informationLayout, Size{~0, 0}, 3};
      Label informationRight{&informationLayout, Size{~0, 0}};
};

struct Presentation : Window {
  Presentation();
  auto clearViewport() -> void;

  MenuBar menuBar{this};
    Menu systemMenu{&menuBar};
      MenuItem load{&systemMenu};
      MenuItem reset{&systemMenu};
      MenuSeparator portSeparator{&systemMenu};
      Menu controllerPort1{&systemMenu};
      Menu controllerPort2{&systemMenu};
      MenuSeparator quitSeparator{&systemMenu};
      MenuItem quit{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu viewMenu{&settingsMenu};
        MenuItem smallView{&viewMenu};
        MenuItem mediumView{&viewMenu};
        MenuItem largeView{&viewMenu};
        MenuSeparator viewSeparator{&viewMenu};
        MenuCheckItem aspectCorrection{&viewMenu};
        MenuCheckItem integralScaling{&viewMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuSeparator muteSeparator{&settingsMenu};
      MenuItem configuration{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveState{&toolsMenu};
      Menu loadState{&toolsMenu};
      MenuCheckItem pauseEmulation{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem about{&helpMenu};

  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 512, 480}};
};

extern unique_pointer<AboutWindow> aboutWindow;
extern unique_pointer<Presentation> presentation;
