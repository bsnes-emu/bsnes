struct CheatEditor : TopLevelWindow {
  ListBox cheatList;
  Label codeLabel;
  TextBox codeEdit;
  Label descLabel;
  TextBox descEdit;
  Button findButton;
  Button clearAllButton;
  Button clearButton;

  TopLevelWindow databaseWindow;
  ListBox databaseList;
  lstring databaseCode;
  Button databaseSelectAll;
  Button databaseUnselectAll;
  Button databaseOk;

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
