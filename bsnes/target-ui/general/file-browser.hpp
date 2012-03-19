struct FileBrowser : Window {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
    LineEdit pathEdit;
    Button pathBrowse;
    Button pathUp;
  ListView fileList;
  HorizontalLayout controlLayout;
  Label filterLabel;
  Button openButton;

  struct Mode { enum : unsigned { Default, NES, SNES, GameBoy, GameBoyColor, GameBoyAdvance, Satellaview, SufamiTurbo }; };
  void open(const string &title, unsigned mode, function<void (string)> callback);

  FileBrowser();
  ~FileBrowser();

private:
  configuration config;
  struct FilterMode {
    string name;
    string path;
    lstring filter;
  } *mode;
  vector<FilterMode> filterModes;

  lstring fileNameList;
  function<void (string)> callback;

  void synchronize();
  void setPath(const string &path);
  void fileListActivate();
  bool loadFolder(const string &path);
  void loadFile(const string &filename);
};

extern FileBrowser *fileBrowser;
