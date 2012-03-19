struct GbController : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, select, start;
  TurboInput bTurbo, aTurbo;

  int16_t poll(unsigned n);
  GbController();
};

struct GbDevice : SecondaryInput {
  GbController controller;

  GbDevice();
};

struct GbInput : PrimaryInput {
  GbDevice device;

  GbInput();
};
