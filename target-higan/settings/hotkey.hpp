struct HotkeySettings : SettingsLayout {
  ListView inputList;
  HorizontalLayout controlLayout;
    Widget spacer;
    Button eraseButton;

  void synchronize();
  void refresh();
  void eraseInput();
  void assignInput();
  void inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  HotkeySettings();

private:
  HotkeyInput* activeInput = nullptr;
};

extern HotkeySettings* hotkeySettings;
