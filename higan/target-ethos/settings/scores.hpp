struct ScoreSettings : SettingsLayout {
  Label title;
  HorizontalLayout serverLayout;
    Label hostLabel;
    LineEdit hostEdit;
  HorizontalLayout loginLayout;
    Label userLabel;
    LineEdit userEdit;
    Label passLabel;
    LineEdit passEdit;

  ScoreSettings();
};

extern ScoreSettings *scoreSettings;
