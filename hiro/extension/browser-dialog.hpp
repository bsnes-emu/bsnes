#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow;

struct BrowserDialog {
  using type = BrowserDialog;

  BrowserDialog();
  auto openFile() -> string;           //one existing file
  auto openFiles() -> vector<string>;  //any existing files
  auto openFolder() -> string;         //one existing folder
  auto openObject() -> string;         //one existing file or folder
  auto option() -> string;
  auto saveFile() -> string;           //one file
  auto selected() -> vector<string>;
  auto selectFolder() -> string;       //one existing folder
  auto setFilters(const vector<string>& filters = {}) -> type&;
  auto setOptions(const vector<string>& options = {}) -> type&;
  auto setParent(const sWindow& parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

private:
  struct State {
    string action;
    vector<string> filters = {"*"};
    vector<string> options;
    sWindow parent;
    string path;
    string title;
  } state;

  struct Response {
    string option;
    vector<string> selected;
  } response;

  auto _run() -> vector<string>;

  friend class BrowserDialogWindow;
};

#endif
