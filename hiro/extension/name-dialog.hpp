#if defined(Hiro_NameDialog)

struct NameDialog {
  using type = NameDialog;

  NameDialog();
  auto create(string name = "") -> string;
  auto rename(string name) -> string;
  auto setIcon(const image& icon = {}) -> type&;
  auto setAlignment(Alignment = Alignment::Center) -> type&;
  auto setAlignment(sWindow relativeTo, Alignment = Alignment::Center) -> type&;
  auto setText(const string& text = {}) -> type&;
  auto setTitle(const string& title = {}) -> type&;

private:
  auto show(string mode, string name) -> string;

  Window window;
    VerticalLayout layout{&window};
      Label textLabel{&layout, Size{~0, 0}};
      HorizontalLayout nameLayout{&layout, Size{~0, 0}};
        Canvas typeIcon{&nameLayout, Size{16_sx, 16_sy}};
        LineEdit nameValue{&nameLayout, Size{~0, 0}};
      HorizontalLayout controlLayout{&layout, Size{~0, 0}};
        Widget controlSpacer{&controlLayout, Size{~0, 0}};
        Button acceptButton{&controlLayout, Size{80, 0}};
        Button cancelButton{&controlLayout, Size{80, 0}};

  struct State {
    Alignment alignment = Alignment::Center;
    image icon;
    sWindow relativeTo;
    string text;
    string title;
  } state;

  string response;
};

#endif
