struct CheatEditor : Window {
  ListBox cheatList;
  Label codeLabel;
  TextBox codeEdit;
  Label descLabel;
  TextBox descEdit;
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
  void clearAll();
  void clear();
};

extern CheatEditor cheatEditor;
