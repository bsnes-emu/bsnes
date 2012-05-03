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
    vector<Action*> loadList;
  Menu settingsMenu;
    Menu videoMenu;
      RadioItem centerVideo;
      RadioItem scaleVideo;
      RadioItem stretchVideo;
      CheckItem aspectCorrection;
    Item configurationSettings;
  Menu toolsMenu;
    Item resizeWindow;

  void synchronize();
  void setSystemName(const string &name);
  void bootstrap();
  Presentation();

private:
  System *activeSystem;
};

extern Presentation *presentation;
