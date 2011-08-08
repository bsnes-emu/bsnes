struct InputSettings : TopLevelWindow {
  VerticalLayout layout;
  HorizontalLayout selectionLayout;
  Label portLabel;
  ComboBox portBox;
  Label deviceLabel;
  ComboBox deviceBox;
  ListView mappingList;
  HorizontalLayout mapLayout;
  Label spacer;
  Button clearButton;

  VerticalLayout axisLayout;
  Widget axisSpacer;
  HorizontalLayout axisControlLayout;
  Button mouseXaxis;
  Button mouseYaxis;

  VerticalLayout buttonLayout;
  Widget buttonSpacer;
  HorizontalLayout buttonControlLayout;
  Button mouseLeft;
  Button mouseMiddle;
  Button mouseRight;

  void inputEvent(uint16_t scancode, int16_t value);
  void calibrateJoypads();
  void create();
  InputSettings();

private:
  bool joypadsCalibrated;
  bool joypadsCalibrating;
  int16_t joypadCalibration[Joypad::Count][Joypad::Axes];
  unsigned activeMouse;

  void portChanged();
  void deviceChanged();
  void mappingChanged();
  void setMapping(const string &mapping);
  void assignInput();
  void clearInput();
  void endAssignment();
};

extern InputSettings inputSettings;
