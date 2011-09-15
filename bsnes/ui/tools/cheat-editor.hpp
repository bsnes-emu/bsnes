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
    Button clearAllButton;
    Button clearButton;

  void synchronize();
  void updateUI();
  void updateInterface();
  void updateCode();
  void updateDesc();
  void clearAll();
  void clearSelected();

  void reset();
  bool load(const string &filename);
  bool save(const string &filename);

  CheatEditor();

private:
  enum : unsigned { Code = 0, Desc = 1 };
  string cheatText[128][2];
};

extern CheatEditor *cheatEditor;
