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

  MenuBar menuBar{this};
    Menu fileMenu{&menuBar};
      MenuItem quit{&fileMenu};
    Menu settingsMenu{&menuBar};
    Menu toolsMenu{&menuBar};
    Menu helpMenu{&menuBar};
      MenuItem about{&helpMenu};

  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};
};

extern unique_pointer<AboutWindow> aboutWindow;
extern unique_pointer<Presentation> presentation;
