struct PathSettings : SettingsLayout {
  Label title;
  HorizontalLayout gameLayout;
    Label gameLabel;
    LineEdit gamePath;
    Button gameBrowse;

  PathSettings();
};

extern PathSettings *pathSettings;
