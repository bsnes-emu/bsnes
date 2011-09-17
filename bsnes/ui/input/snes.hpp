struct SnesGamepad : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, y, x;
  DigitalInput l, r, select, start;

  int16_t poll(unsigned n);
  SnesGamepad();
};

struct SnesPort1Input : SecondaryInput {
  SnesGamepad gamepad;

  SnesPort1Input();
};

struct SnesPort2Input : SecondaryInput {
  SnesGamepad gamepad;

  SnesPort2Input();
};

struct SnesInput : PrimaryInput {
  SnesPort1Input port1;
  SnesPort2Input port2;

  SnesInput();
};
