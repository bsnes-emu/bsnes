struct MessageDialog {
  using type = MessageDialog;

  MessageDialog(const nall::string& text = "");
  auto error(const nall::lstring& buttons = {"Ok"}) -> signed;
  auto information(const nall::lstring& buttons = {"Ok"}) -> signed;
  auto question(const nall::lstring& buttons = {"Yes", "No"}) -> signed;
  auto setParent(nall::shared_pointer<mWindow> parent = {}) -> type&;
  auto setText(const nall::string& text = "") -> type&;
  auto setTitle(const nall::string& title = "") -> type&;
  auto warning(const nall::lstring& buttons = {"Ok"}) -> signed;

private:
  struct State {
    nall::lstring buttons;
    nall::vector<uint8_t> icon;
    nall::shared_pointer<mWindow> parent;
    signed response = -1;
    nall::string text;
    nall::string title;
  } state;

  auto _run() -> signed;
};
