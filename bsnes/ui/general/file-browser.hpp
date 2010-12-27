struct FileBrowser : TopLevelWindow {
  TextBox pathBox;
  Button browseButton;
  Button upButton;
  ListBox contentsBox;

  enum class Mode : unsigned { Cartridge, Satellaview, SufamiTurbo, GameBoy, Filter, Shader } mode;
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
  void setFolder(const string &pathname);
  string cartridgeFolder(const string &pathname);
  void loadFile(const string &filename);
};

extern FileBrowser fileBrowser;
