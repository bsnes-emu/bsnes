struct AdvancedSettings : TopLevelWindow {
  VerticalLayout layout;
  Label driverSelectionLabel;
  HorizontalLayout driverLayout;
  Label videoDriverLabel;
  ComboBox videoDriverBox;
  Label audioDriverLabel;
  ComboBox audioDriverBox;
  Label inputDriverLabel;
  ComboBox inputDriverBox;
  Label focusPolicyLabel;
  HorizontalLayout focusPolicyLayout;
  RadioBox focusPolicyPause;
  RadioBox focusPolicyIgnore;
  RadioBox focusPolicyAllow;

  void create();
};

extern AdvancedSettings advancedSettings;
