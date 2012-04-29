struct AbstractInput {
  enum class Type : unsigned { Button, MouseButton, MouseAxis, HatUp, HatDown, HatLeft, HatRight, Axis, AxisLo, AxisHi } type;
  string name;
  string mapping;
  unsigned scancode;

  void bind();
};

struct InputManager {
  int16_t table[Scancode::Limit];

  map<unsigned, AbstractInput> inputMap;

  void bind();
  void poll();
  int16_t poll(unsigned guid);
  void bootstrap();
  InputManager();

private:
  configuration config;
};

extern InputManager *inputManager;
