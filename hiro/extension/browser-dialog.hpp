#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow;

struct BrowserDialog {
  using type = BrowserDialog;

  BrowserDialog();
  auto openFile() -> string;          //one existing file
  auto openFiles() -> string_vector;  //any existing files
  auto openFolder() -> string;        //one existing folder
  auto openObject() -> string;        //one existing file or folder
  auto option() -> string;
  auto saveFile() -> string;          //one file
  auto selected() -> string_vector;
  auto selectFolder() -> string;      //one existing folder
  auto setFilters(const string_vector& filters = {}) -> type&;
  auto setOptions(const string_vector& options = {}) -> type&;
  auto setParent(const sWindow& parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

private:
  struct State {
    string action;
    string_vector filters = {"*"};
    string_vector options;
    sWindow parent;
    string path;
    string title;
  } state;

  struct Response {
    string option;
    string_vector selected;
  } response;

  auto _run() -> string_vector;

  friend class BrowserDialogWindow;
};

#endif
