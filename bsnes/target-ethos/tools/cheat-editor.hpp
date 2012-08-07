struct CheatEditor : Window {
  VerticalLayout layout;
  ListView cheatList;
  HorizontalLayout codeLayout;
    Label codeLabel;
    LineEdit codeEdit;
  HorizontalLayout descLayout;
    Label descLabel;
    LineEdit descEdit;
  HorizontalLayout controlLayout;
    Button findButton;
    Widget spacer;
    Button resetButton;
    Button eraseButton;

  void reset();
  void erase();
  void updateCode();
  void updateDesc();

  bool load(const string &filename);
  bool save(const string &filename);
  bool import(const string &code, const string &desc);

  void update();
  void refresh();
  void synchronize();
  CheatEditor();

private:
  enum : unsigned { Codes = 128 };
  struct Cheat {
    string code;
    string desc;
  };
  Cheat cheat[Codes];
};

extern CheatEditor *cheatEditor;
