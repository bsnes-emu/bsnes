struct InputSettings : Window {
  Label portLabel;
  ComboBox portBox;
  Label deviceLabel;
  ComboBox deviceBox;
  ListBox mappingList;

  void inputEvent(uint16_t scancode, int16_t value);
  void calibrateJoypads();
  void create();
  InputSettings();

private:
  bool joypadsCalibrated;
  bool joypadsCalibrating;
  int16_t joypadCalibration[Joypad::Count][Joypad::Axes];

  void portChanged();
  void deviceChanged();
  void setMapping(const char *mapping);
  void assignInput();
};

extern InputSettings inputSettings;
