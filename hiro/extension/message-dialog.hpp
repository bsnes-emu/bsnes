#if defined(Hiro_MessageDialog)

struct MessageDialog {
  using type = MessageDialog;

  MessageDialog(const string& text = "");
  auto error(const string_vector& buttons = {"Ok"}) -> string;
  auto information(const string_vector& buttons = {"Ok"}) -> string;
  auto question(const string_vector& buttons = {"Yes", "No"}) -> string;
  auto setParent(sWindow parent = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto warning(const string_vector& buttons = {"Ok"}) -> string;

private:
  struct State {
    string_vector buttons;
    vector<uint8_t> icon;
    sWindow parent;
    string response;
    string text;
    string title;
  } state;

  auto _run() -> string;
};

#endif
