struct CheatEditor : TopLevelWindow {
  VerticalLayout layout;
  ListBox cheatList;
  HorizontalLayout codeLayout;
  Label codeLabel;
  LineEdit codeEdit;
  HorizontalLayout descLayout;
  Label descLabel;
  LineEdit descEdit;
  HorizontalLayout controlLayout;
  Label spacer;
  Button findButton;
  Button clearAllButton;
  Button clearButton;

  TopLevelWindow databaseWindow;
  VerticalLayout databaseLayout;
  ListBox databaseList;
  HorizontalLayout databaseControlLayout;
  Button databaseSelectAll;
  Button databaseUnselectAll;
  Label databaseSpacer;
  Button databaseOk;
  lstring databaseCode;

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
  void findCodes();
  optional<unsigned> findUnusedSlot();
  void addDatabaseCodes();
  void clearAll();
  void clear();
};

extern CheatEditor cheatEditor;
