struct Presentation : Window {
  FixedLayout layout;
  Viewport viewport;

  struct System {
    Emulator::Interface *interface;

    Menu menu;
      Item power;
      Item reset;
      Separator separator;
      Item unload;
    function<void (string)> callback;
  };
  vector<System*> emulatorList;

  Menu loadMenu;
    vector<Item*> loadList;
  Menu settingsMenu;
    Item configurationSettings;
  Menu toolsMenu;

  void synchronize();
  void setSystemName(const string &name);
  void bootstrap();
  Presentation();

private:
  System *activeSystem;
};

extern Presentation *presentation;
