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
  lstring cheatCode;
};

extern CheatDatabase *cheatDatabase;
