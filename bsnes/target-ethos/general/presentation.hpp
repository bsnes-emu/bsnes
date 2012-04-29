struct Presentation : Window {
  FixedLayout layout;
  Viewport viewport;

  struct System {
    Emulator::Interface *interface;

    string name;
    string filter;
    Item load;
    Menu menu;
      Item power;
      Item reset;
      Separator separator;
      Item unload;
    function<void (string)> callback;
  };
  vector<System*> emulatorList;

  Menu loadMenu;
  Menu settingsMenu;
    Item configurationSettings;
  Menu toolsMenu;

  void synchronize();
  void bootstrap();
  Presentation();
};

extern Presentation *presentation;
