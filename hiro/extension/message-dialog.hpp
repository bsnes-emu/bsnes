#if defined(Hiro_MessageDialog)

struct MessageDialog {
  using type = MessageDialog;

  MessageDialog(const string& text = "");
  auto error(const vector<string>& buttons = {"Ok"}) -> string;
  auto information(const vector<string>& buttons = {"Ok"}) -> string;
  auto question(const vector<string>& buttons = {"Yes", "No"}) -> string;
  auto setParent(sWindow parent = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto warning(const vector<string>& buttons = {"Ok"}) -> string;

private:
  struct State {
    vector<string> buttons;
    vector<uint8_t> icon;
    sWindow parent;
    string response;
    string text;
    string title;
  } state;

  auto _run() -> string;
};

#endif
