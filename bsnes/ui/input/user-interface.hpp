struct HotkeyGeneral : TertiaryInput {
  DigitalInput saveState;
  DigitalInput loadState;
  DigitalInput decrementSlot;
  DigitalInput incrementSlot;
  DigitalInput toggleMouseCapture;
  DigitalInput toggleFullScreen;
  DigitalInput pause;
  DigitalInput turboMode;
  DigitalInput power;
  DigitalInput reset;
  DigitalInput quit;

  void inputEvent(int16_t scancode, int16_t value);
  HotkeyGeneral();

private:
  unsigned activeSlot;
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
