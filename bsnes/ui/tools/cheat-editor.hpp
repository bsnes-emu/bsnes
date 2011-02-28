struct CheatEditor : TopLevelWindow {
  VerticalLayout layout;
  ListView cheatList;
  HorizontalLayout codeLayout;
  Label codeLabel;
  LineEdit codeEdit;
  HorizontalLayout descLayout;
  Label descLabel;
  LineEdit descEdit;
  HorizontalLayout controlLayout;
  Widget spacerWidget;
  Button findButton;
  Button clearAllButton;
  Button clearButton;

  void load(string filename);
  void save(string filename);
  void create();

private:
  enum : unsigned { CheatSlot, CheatCode, CheatDesc };
  string cheatText[128][3];
  void synchronize();
  void refresh();
  void toggle(unsigned row);
  void bind();
  optional<unsigned> findUnusedSlot();
  void clearAll();
  void clear();

  friend class CheatDatabase;
};

extern CheatEditor cheatEditor;
