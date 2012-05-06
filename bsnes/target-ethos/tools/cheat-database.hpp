struct CheatDatabase : Window {
  VerticalLayout layout;
  ListView cheatList;
  HorizontalLayout controlLayout;
    Button selectAllButton;
    Button unselectAllButton;
    Widget spacer;
    Button acceptButton;

  void findCodes();
  void addCodes();
  CheatDatabase();

private:
  struct Cheat {
    string code;
    string desc;
  };
  vector<Cheat> cheat;
};

extern CheatDatabase *cheatDatabase;
