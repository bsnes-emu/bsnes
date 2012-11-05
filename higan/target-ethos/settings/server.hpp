struct ServerSettings : SettingsLayout {
  Label title;
  HorizontalLayout hostLayout;
    Label hostLabel;
    LineEdit hostEdit;
  HorizontalLayout userLayout;
    Label userLabel;
    LineEdit userEdit;
    Label passLabel;
    LineEdit passEdit;

  ServerSettings();
};

extern ServerSettings *serverSettings;
