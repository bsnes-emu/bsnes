struct HotkeySettings : SettingsLayout {
  Label title;
  ListView inputList;
  HorizontalLayout controlLayout;
    Widget spacer;
    Button clearButton;

  void synchronize();
  void refresh();
  void clearInput();
  void assignInput();
  void inputEvent(unsigned scancode, int16_t value);
  HotkeySettings();

private:
  HotkeyInput *activeInput;
};

extern HotkeySettings *hotkeySettings;
