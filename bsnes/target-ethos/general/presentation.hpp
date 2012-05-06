struct Presentation : Window {
  FixedLayout layout;
  Viewport viewport;

  struct Emulator {
    ::Emulator::Interface *interface;

    Menu menu;
      Item power;
      Item reset;
      Item unload;
      Separator controllerSeparator;
      struct Port {
        Menu menu;
        set<RadioItem&> group;
        vector<RadioItem*> device;
      };
      vector<Port*> port;
    function<void (string)> callback;
  };
  vector<Emulator*> emulatorList;

  Menu loadMenu;
    vector<Item*> loadListSystem;
    vector<Item*> loadListSubsystem;
  Menu settingsMenu;
    Menu videoMenu;
      RadioItem centerVideo;
      RadioItem scaleVideo;
      RadioItem stretchVideo;
      CheckItem aspectCorrection;
      CheckItem maskOverscan;
    CheckItem synchronizeVideo;
    CheckItem synchronizeAudio;
    CheckItem muteAudio;
    Item configurationSettings;
  Menu toolsMenu;
    Menu saveStateMenu;
      Item saveStateItem[5];
    Menu loadStateMenu;
      Item loadStateItem[5];
    Separator stateMenuSeparator;
    Item resizeWindow;
    Item cheatEditor;
    Item stateManager;

  void synchronize();
  void setSystemName(const string &name);
  void bootstrap();
  Presentation();

private:
  Emulator *active;
};

extern Presentation *presentation;
