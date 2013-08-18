struct DipSwitch : HorizontalLayout {
  Label name;
  ComboButton value;
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

  unsigned run(const Markup::Node& node);
  DipSwitches();
};

extern DipSwitches* dipSwitches;
