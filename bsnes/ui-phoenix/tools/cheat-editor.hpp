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
  string cheatText[128][4];
  void synchronize();
  void refresh();
  void toggle();
  void bind();
  void clearAll();
  void clear();
};

extern CheatEditor cheatEditor;
