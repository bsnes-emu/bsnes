#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow;

struct BrowserDialog {
  using type = BrowserDialog;

  BrowserDialog();
  auto openFile() -> string;          //one existing file
  auto openFiles() -> string_vector;  //any existing files or folders
  auto openFolder() -> string;        //one existing folder
  auto saveFile() -> string;          //one file
  auto selectFolder() -> string;      //one existing folder
  auto setFilters(const string_vector& filters = {}) -> type&;
  auto setParent(const sWindow& parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

private:
  struct State {
    string action;
    string_vector filters = {"*"};
    sWindow parent;
    string path;
    string_vector response;
    string title;
  } state;

  auto _run() -> string_vector;

  friend class BrowserDialogWindow;
};

#endif
