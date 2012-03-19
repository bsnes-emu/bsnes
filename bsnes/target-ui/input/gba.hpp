struct GbaController : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, l, r;
  DigitalInput select, start;
  TurboInput bTurbo, aTurbo, lTurbo, rTurbo;

  int16_t poll(unsigned n);
  GbaController();
};

struct GbaDevice : SecondaryInput {
  GbaController controller;

  GbaDevice();
};

struct GbaInput : PrimaryInput {
  GbaDevice device;

  GbaInput();
};
