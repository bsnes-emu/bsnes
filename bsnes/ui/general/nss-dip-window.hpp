struct NSSDipWindow : TopLevelWindow {
  FixedLayout layout;
  Label dipName[16];
  ComboBox dipValue[16];
  Button loadButton;

  void create();
  void select();
  void assign();
};

extern NSSDipWindow nssDipWindow;
