struct SnesGamepad : TertiaryInput {
  DigitalInput up, down, left, right;
  DigitalInput b, a, y, x;
  DigitalInput l, r, select, start;

  int16_t poll(unsigned n);
  SnesGamepad(const string &name, bool defaultBindings);
};

struct SnesMouse : TertiaryInput {
  AnalogInput xaxis, yaxis;
  DigitalInput left, right;

  int16_t poll(unsigned n);
  SnesMouse(const string &name, bool defaultBindings);
};

struct SnesSuperScope : TertiaryInput {
  AnalogInput xaxis, yaxis;
  DigitalInput trigger, cursor, turbo, pause;

  int16_t poll(unsigned n);
  SnesSuperScope(const string &name, bool defaultBindings);
};

struct SnesJustifier : TertiaryInput {
  AnalogInput xaxis, yaxis;
  DigitalInput trigger, start;

  int16_t poll(unsigned n);
  SnesJustifier(const string &name, bool defaultBindings);
};

struct SnesPort1Input : SecondaryInput {
  SnesGamepad gamepad;
  SnesGamepad multitap1;
  SnesGamepad multitap2;
  SnesGamepad multitap3;
  SnesGamepad multitap4;
  SnesMouse mouse;

  SnesPort1Input();
};

struct SnesPort2Input : SecondaryInput {
  SnesGamepad gamepad;
  SnesGamepad multitap1;
  SnesGamepad multitap2;
  SnesGamepad multitap3;
  SnesGamepad multitap4;
  SnesMouse mouse;
  SnesSuperScope superScope;
  SnesJustifier justifier1;
  SnesJustifier justifier2;

  SnesPort2Input();
};

struct SnesInput : PrimaryInput {
  SnesPort1Input port1;
  SnesPort2Input port2;

  SnesInput();
};
