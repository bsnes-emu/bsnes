struct FileBrowser : Window {
  TextBox pathBox;
  Button browseButton;
  Button upButton;
  ListBox contentsBox;

  void fileOpen(const char *pathname);
  void create();

private:
  string folder;
  lstring contents;

  void folderBrowse();
  void folderUp();
  void fileActivate();
  void setFolder(const char *pathname);
};

extern FileBrowser fileBrowser;
