struct InputSettings : SettingsLayout {
  Label title;
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
  InputSettings();
};

extern InputSettings *inputSettings;
