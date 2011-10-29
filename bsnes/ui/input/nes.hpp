struct NesGamepad : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, select, start;
  TurboInput bTurbo, aTurbo;

  int16_t poll(unsigned n);
  NesGamepad();
};

struct NesPort1Input : SecondaryInput {
  NesGamepad gamepad;

  NesPort1Input();
};

struct NesPort2Input : SecondaryInput {
  NesGamepad gamepad;

  NesPort2Input();
};

struct NesInput : PrimaryInput {
  NesPort1Input port1;
  NesPort2Input port2;

  NesInput();
};
