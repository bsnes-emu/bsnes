#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow;

struct BrowserDialog {
  using type = BrowserDialog;

  BrowserDialog();
  auto openFile() -> string;      //one existing file
  auto openFiles() -> lstring;    //any existing files or folders
  auto openFolder() -> string;    //one existing folder
  auto saveFile() -> string;      //one file
  auto selectFolder() -> string;  //one existing folder
  auto setFilters(const lstring& filters = {}) -> type&;
  auto setParent(const sWindow& parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

private:
  struct State {
    string action;
    lstring filters = {"*"};
    sWindow parent;
    string path;
    lstring response;
    string title;
  } state;

  auto _run() -> lstring;

  friend class BrowserDialogWindow;
};

#endif
