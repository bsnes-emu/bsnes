#if defined(Hiro_MessageDialog)

struct MessageDialog {
  using type = MessageDialog;

  MessageDialog(const string& text = "");
  auto error(const lstring& buttons = {"Ok"}) -> string;
  auto information(const lstring& buttons = {"Ok"}) -> string;
  auto question(const lstring& buttons = {"Yes", "No"}) -> string;
  auto setParent(sWindow parent = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto warning(const lstring& buttons = {"Ok"}) -> string;

private:
  struct State {
    lstring buttons;
    vector<uint8_t> image;
    sWindow parent;
    string response;
    string text;
    string title;
  } state;

  auto _run() -> string;
};

#endif
