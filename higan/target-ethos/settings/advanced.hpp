struct AdvancedSettings : SettingsLayout {
  Label driverTitle;
  HorizontalLayout driverLayout;
    Label videoLabel;
    ComboButton videoDriver;
    Label audioLabel;
    ComboButton audioDriver;
    Label inputLabel;
    ComboButton inputDriver;

  Label libraryTitle;
  HorizontalLayout libraryLayout;
    Label libraryLabel;
    LineEdit libraryPath;
    Button libraryBrowse;

  Widget spacer;
  Label infoLabel;

  AdvancedSettings();
};

extern AdvancedSettings* advancedSettings;
