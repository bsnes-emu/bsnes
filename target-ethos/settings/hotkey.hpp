struct HotkeySettings : SettingsLayout {
  Label title;
  ListView inputList;
  HorizontalLayout controlLayout;
    Widget spacer;
    Button eraseButton;

  void synchronize();
  void refresh();
  void eraseInput();
  void assignInput();
  void inputEvent(unsigned scancode, int16_t value);
  HotkeySettings();

private:
  HotkeyInput* activeInput = nullptr;
};

extern HotkeySettings* hotkeySettings;
