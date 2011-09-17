struct HotkeyGeneral : TertiaryInput {
  DigitalInput toggleFullScreen;
  DigitalInput turboMode;

  void inputEvent(int16_t scancode, int16_t value);
  HotkeyGeneral();
};

struct HotkeyInput : SecondaryInput {
  HotkeyGeneral general;

  void inputEvent(int16_t scancode, int16_t value);
  HotkeyInput();
};

struct UserInterfaceInput : PrimaryInput {
  HotkeyInput hotkey;

  void inputEvent(int16_t scancode, int16_t value);
  UserInterfaceInput();
};
