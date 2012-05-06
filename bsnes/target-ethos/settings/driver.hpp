struct DriverSettings : SettingsLayout {
  Label title;
  HorizontalLayout driverLayout;
    Label videoLabel;
    ComboBox videoDriver;
    Label audioLabel;
    ComboBox audioDriver;
    Label inputLabel;
    ComboBox inputDriver;

  DriverSettings();
};

extern DriverSettings *driverSettings;
