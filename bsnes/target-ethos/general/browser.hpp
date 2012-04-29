struct Browser : Window {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
    LineEdit pathEdit;
    Button pathBrowse;
    Button pathUp;
  ListView fileList;
  HorizontalLayout controlLayout;
  Label filterLabel;
  Button openButton;

  void open(Emulator::Interface::Media &media, function<void (string)> callback);
  Browser();

public:
  Emulator::Interface::Media media;
  function<void (string)> callback;
  string path;
  lstring filenameList;

  void synchronize();
  void setPath(const string &path);
  void fileListActivate();
  bool loadFolder(const string &path);
  void loadFile(const string &filename);
};

extern Browser *browser;
