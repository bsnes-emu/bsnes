struct InputSettings : VerticalLayout {
  Label title;
  HorizontalLayout selectionLayout;
    ComboBox primary;
    ComboBox secondary;
    ComboBox tertiary;
  ListView inputList;
  HorizontalLayout controlLayout;
    Button assignPrimary;
    Button assignSecondary;
    Button assignTertiary;
    Widget spacer;
    Button clearButton;

  InputSettings();

  void synchronize();
  void primaryChange();
  void secondaryChange();
  void tertiaryChange();
  void assignInput();
  void assignMouseInput(unsigned);
  void clearInput();
  void inputEvent(int16_t scancode, int16_t value, bool allowMouseInput = false);

private:
  AbstractInput *activeInput;
};

extern InputSettings *inputSettings;
