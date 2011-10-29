struct GameBoyController : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, select, start;
  TurboInput bTurbo, aTurbo;

  int16_t poll(unsigned n);
  GameBoyController();
};

struct GameBoyDevice : SecondaryInput {
  GameBoyController controller;

  GameBoyDevice();
};

struct GameBoyInput : PrimaryInput {
  GameBoyDevice device;

  GameBoyInput();
};
