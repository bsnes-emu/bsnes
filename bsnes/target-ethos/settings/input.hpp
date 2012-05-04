struct InputSettings : SettingsLayout {
  Label title;
  HorizontalLayout focusLayout;
    Label focusLabel;
    CheckBox focusPause;
    CheckBox focusAllow;
  HorizontalLayout selectionLayout;
    ComboBox systemList;
    ComboBox portList;
    ComboBox deviceList;
  ListView inputList;
  HorizontalLayout controlLayout;
    Button assign[3];
    Widget spacer;
    Button clearButton;

  void synchronize();

  Emulator::Interface& activeSystem();
  Emulator::Interface::Port& activePort();
  Emulator::Interface::Port::Device& activeDevice();

  void systemChanged();
  void portChanged();
  void deviceChanged();
  void clearInput();
  void assignInput();
  void assignMouseInput(unsigned n);
  void inputEvent(unsigned scancode, int16_t value, bool allowMouseInput = false);
  InputSettings();

private:
  AbstractInput *activeInput;
};

extern InputSettings *inputSettings;
