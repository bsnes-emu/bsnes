struct Presentation : Window {
  FixedLayout layout;
  Viewport viewport;

  struct Emulator {
    ::Emulator::Interface* interface;

    Menu menu;
      Item power;
      Item reset;
      Item unload;
      Separator controllerSeparator;
      struct Port {
        Menu menu;
        nall::group<RadioItem> group;
        vector<RadioItem*> device;
      };
      vector<Port*> port;
    function<void (string)> callback;
  };
  vector<Emulator*> emulatorList;
  Emulator* active = nullptr;

  Menu loadMenu;
    vector<Item*> loadListSystem;
    Separator loadSeparator;
    Item loadImport;
  Menu settingsMenu;
    Menu videoMenu;
      RadioItem centerVideo;
      RadioItem scaleVideo;
      RadioItem stretchVideo;
      CheckItem aspectCorrection;
      CheckItem maskOverscan;
    Menu shaderMenu;
      RadioItem shaderNone;
      RadioItem shaderBlur;
      vector<RadioItem*> shaderList;
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
    Item stateManager;
    Item cheatEditor;
    Item synchronizeTime;

  void synchronize();
  void setSystemName(string name);
  void loadShaders();
  void bootstrap();
  Presentation();
};

extern Presentation* presentation;
