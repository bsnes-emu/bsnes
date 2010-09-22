struct AdvancedSettingsWindow : Window {
  Label driverSelectionLabel;
  Label videoDriverLabel;
  ComboBox videoDriverBox;
  Label audioDriverLabel;
  ComboBox audioDriverBox;
  Label inputDriverLabel;
  ComboBox inputDriverBox;

  void create();
};

extern AdvancedSettingsWindow advancedSettingsWindow;
