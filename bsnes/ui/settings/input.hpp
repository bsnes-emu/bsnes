struct InputSettings : VerticalLayout {
  Label title;
  HorizontalLayout selectionLayout;
  ComboBox primary;
  ComboBox secondary;
  ComboBox tertiary;
  ListView inputList;
  HorizontalLayout controlLayout;
  Widget spacer;
  Button clearButton;

  InputSettings();

  void synchronize();
  void primaryChange();
  void secondaryChange();
  void tertiaryChange();
  void assignInputBegin();
  void inputEvent(int16_t scancode, int16_t value);

private:
  AbstractInput *activeInput;
};

extern InputSettings *inputSettings;
