struct Browser : Window {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
    LineEdit pathEdit;
    Button homeButton;
    Button upButton;
  ListView fileList;
  HorizontalLayout controlLayout;
  Label filterLabel;
  Button openButton;

  string select(string title, string extension);
  void saveConfiguration();
  void synchronize();
  void bootstrap();
  Browser();

private:
  Configuration::Document config;
  struct Folder {
    string extension;
    string path;
    unsigned selection;
  };
  vector<Folder> folderList;

  string outputFilename;

  string extension;
  string path;
  lstring filenameList;

  void setPath(string path, unsigned selection = 0);
  void fileListActivate();
};

extern Browser* browser;
