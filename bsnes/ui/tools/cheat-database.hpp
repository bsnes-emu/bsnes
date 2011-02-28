struct CheatDatabase : TopLevelWindow {
  VerticalLayout layout;
  ListView listView;
  HorizontalLayout controlLayout;
  Button selectAllButton;
  Button unselectAllButton;
  Widget spacerWidget;
  Button okButton;
  lstring listData;

  void create();
  void findCodes();
  void addCodes();
};

extern CheatDatabase cheatDatabase;
