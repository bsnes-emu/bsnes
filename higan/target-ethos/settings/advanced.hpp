struct AdvancedSettings : SettingsLayout {
  Label driverTitle;
  HorizontalLayout driverLayout;
    Label videoLabel;
    ComboBox videoDriver;
    Label audioLabel;
    ComboBox audioDriver;
    Label inputLabel;
    ComboBox inputDriver;

  Label libraryTitle;
  HorizontalLayout libraryLayout;
    Label libraryLabel;
    LineEdit libraryPath;
    Button libraryBrowse;

  Widget spacer;
  Label infoLabel;

  AdvancedSettings();
};

extern AdvancedSettings *advancedSettings;
