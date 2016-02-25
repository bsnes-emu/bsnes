struct Terminal : Window {
  Terminal();
  auto showAboutWindow() -> void;

  MenuBar menuBar{this};
    Menu helpMenu{&menuBar};
      MenuItem aboutAction{&helpMenu};

  VerticalLayout layout{this};
    Console console{&layout, Size{~0, ~0}};
};

struct AboutWindow : Window {
  AboutWindow();

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{288, 360}, 15};
    Label information{&layout, Size{~0, 0}};
};

extern unique_pointer<Terminal> terminal;
extern unique_pointer<AboutWindow> aboutWindow;
