struct AdvancedSettings : Window {
  Label driverSelectionLabel;
  Label videoDriverLabel;
  ComboBox videoDriverBox;
  Label audioDriverLabel;
  ComboBox audioDriverBox;
  Label inputDriverLabel;
  ComboBox inputDriverBox;
  Label focusPolicyLabel;
  RadioBox focusPolicyPause;
  RadioBox focusPolicyIgnore;
  RadioBox focusPolicyAllow;

  void create();
};

extern AdvancedSettings advancedSettings;
