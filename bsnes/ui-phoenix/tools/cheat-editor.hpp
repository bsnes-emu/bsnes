struct CheatEditor : Window {
  ListBox cheatList;
  Label codeLabel;
  TextBox codeEdit;
  Label descLabel;
  TextBox descEdit;

  void load(string filename);
  void save(string filename);
  void create();

private:
  string cheatText[128][4];
  void synchronize();
  void refresh();
  void toggle();
  void bind();
};

extern CheatEditor cheatEditor;
