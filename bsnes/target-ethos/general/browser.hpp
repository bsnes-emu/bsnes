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

  string select(const string &title, const string &extension);
  void inputEvent(unsigned scancode, int16_t value);
  void saveConfiguration();
  void synchronize();
  void bootstrap();
  Browser();

private:
  configuration config;
  struct Folder {
    string extension;
    string path;
    unsigned selection;
  };
  vector<Folder> folderList;

  bool dialogActive;
  string outputFilename;

  string extension;
  string path;
  lstring filenameList;

  void setPath(const string &path, unsigned selection = 0);
  void fileListActivate();
};

extern Browser *browser;
