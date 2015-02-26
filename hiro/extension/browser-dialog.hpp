struct BrowserDialogWindow;

struct BrowserDialog {
  using type = BrowserDialog;

  BrowserDialog();
  auto openFile() -> nall::string;      //one existing file
  auto openFiles() -> nall::lstring;    //any existing files or folders
  auto saveFile() -> nall::string;      //one file
  auto selectFolder() -> nall::string;  //one existing folder
  auto setFilters(const nall::lstring& filters = {"All|*"}) -> type&;
  auto setParent(const nall::shared_pointer<mWindow>& parent) -> type&;
  auto setPath(const nall::string& path = "") -> type&;
  auto setTitle(const nall::string& title = "") -> type&;

private:
  struct State {
    nall::string action;
    nall::lstring filters = {"*"};
    nall::shared_pointer<mWindow> parent;
    nall::string path;
    nall::lstring response;
    nall::string title;
  } state;

  auto _run() -> nall::lstring;

  friend class BrowserDialogWindow;
};
