struct LibraryBrowser : VerticalLayout {
  ListView folders;
  HorizontalLayout informationLayout;
    Label informationType;
    Label information;
    ComboButton mediaMode;

  LibraryBrowser(Emulator::Interface& emulator);
  void onActivate();
  void onChange();
  void refresh();
  void setMode();

  Emulator::Interface& emulator;
  string pathname;
  string type;
  string typeMask;
  string typeSuffix;
};

struct LibraryImport : VerticalLayout {
  Label information;
  Button importButton;

  LibraryImport();
  void onImportActivate();
};

struct LibraryManager : Window {
  VerticalLayout layout;
  TabFrame libraryFrame;
    vector<LibraryBrowser*> browsers;
    LibraryImport libraryImport;
  HorizontalLayout informationLayout;
    Label information;
    Button skipButton;
    Button loadButton;

  LibraryManager();
  void bootstrap();
  string load(const string& type);
  void onChange();
  void setInformation(bool load);
  void show();
  void synchronize();

  lstring loaded;
  string requestedLoadType;
  bool slotLoad = false;
  string loadPathname;
};

extern LibraryManager* libraryManager;
