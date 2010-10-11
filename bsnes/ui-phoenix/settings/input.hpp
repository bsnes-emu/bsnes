struct InputSettings : TopLevelWindow {
  Label portLabel;
  ComboBox portBox;
  Label deviceLabel;
  ComboBox deviceBox;
  ListBox mappingList;
  Button mouseXaxis;
  Button mouseYaxis;
  Button mouseLeft;
  Button mouseMiddle;
  Button mouseRight;
  Button clearButton;

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
  void setMapping(const char *mapping);
  void assignInput();
  void clearInput();
  void endAssignment();
};

extern InputSettings inputSettings;
