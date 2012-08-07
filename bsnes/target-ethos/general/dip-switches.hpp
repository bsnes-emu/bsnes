struct DipSwitch : HorizontalLayout {
  Label name;
  ComboBox value;
  vector<unsigned> values;

  DipSwitch();
};

struct DipSwitches : Window {
  enum : unsigned { Slots = 8 };

  VerticalLayout layout;
  DipSwitch dip[Slots];
  HorizontalLayout controlLayout;
    Widget spacer;
    Button accept;

  unsigned run(const XML::Node &node);
  DipSwitches();

private:
  bool quit;
};

extern DipSwitches *dipSwitches;
