struct InputSettings {
  HorizontalLayout panelLayout;
  Widget panel;
  VerticalLayout layout;
  Label title;

  HorizontalLayout selectionLayout;
  Label portLabel;
  ComboBox portBox;
  Label deviceLabel;
  ComboBox deviceBox;

  ListView mappingList;

  HorizontalLayout controlLayout;
  Button customButton1;
  Button customButton2;
  Button customButton3;
  Widget spacer;
  Button clearButton;

  void create();

//

  bool joypadsCalibrated;
  bool joypadsCalibrating;
  int16_t joypadCalibration[Joypad::Count][Joypad::Axes];
  unsigned activeMouse;

  void portChanged();
  void deviceChanged();
  void mappingChanged();
  void assignInput();
  void manualInput(unsigned button);
  void clearInput();
  void setMapping(const string &mapping);
  void endAssignment();
  void inputEvent(uint16_t scancode, int16_t value);
  void calibrateJoypads();
};

extern InputSettings inputSettings;
