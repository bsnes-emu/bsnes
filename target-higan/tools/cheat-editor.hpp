struct CheatEditor : ToolsLayout {
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

  bool load(string filename);
  bool save(string filename);
  bool import(string code, string desc);

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

extern CheatEditor* cheatEditor;
