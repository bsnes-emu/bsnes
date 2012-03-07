struct DipSwitch : HorizontalLayout {
  Label name;
  ComboBox value;

  DipSwitch();
};

struct DipSwitches : Window {
  VerticalLayout layout;
  DipSwitch dip[8];
  HorizontalLayout controlLayout;
  Widget spacer;
  Button acceptButton;

  void load();
  void accept();
  DipSwitches();

private:
  unsigned values[8][16];
};

extern DipSwitches *dipSwitches;
