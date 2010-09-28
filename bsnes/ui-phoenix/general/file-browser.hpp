struct FileBrowser : Window {
  TextBox pathBox;
  Button browseButton;
  Button upButton;
  ListBox contentsBox;

  enum class Mode : unsigned { Cartridge, Satellaview, SufamiTurbo, GameBoy } mode;
  void fileOpen(Mode mode, function<void (string)> callback);
  void create();

private:
  function<void (string)> callback;
  string folder;
  lstring filters;
  lstring contents;

  void folderBrowse();
  void folderUp();
  void fileActivate();
  void setFolder(const char *pathname);
};

extern FileBrowser fileBrowser;
