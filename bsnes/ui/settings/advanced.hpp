struct AdvancedSettings : VerticalLayout {
  Label title;
  Label driverLabel;
  HorizontalLayout driverLayout;
    Label videoLabel;
    ComboBox videoDriver;
    Label audioLabel;
    ComboBox audioDriver;
    Label inputLabel;
    ComboBox inputDriver;
  Label focusPolicyLabel;
  HorizontalLayout focusPolicyLayout;
    RadioBox focusPolicy[3];

  AdvancedSettings();
};

extern AdvancedSettings *advancedSettings;
