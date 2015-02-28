struct LibraryBrowser : TabFrameItem {
  LibraryBrowser(TabFrame& parent, Emulator::Interface::Media& media);
  auto reload() -> void;
  auto select() -> void;

  Emulator::Interface::Media media;

  VerticalLayout layout{this};
    ListView gameList{&layout, Size{~0, ~0}};
};

struct LibraryManager : Window {
  LibraryManager();
  auto show(const string& type) -> void;

  VerticalLayout layout{this};
    TabFrame libraryFrame{&layout, Size{~0, ~0}};
      vector<LibraryBrowser*> libraryBrowsers;
};

extern LibraryManager* libraryManager;
