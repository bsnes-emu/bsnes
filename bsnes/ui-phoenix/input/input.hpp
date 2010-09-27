struct InputMapper {
  struct AbstractInput {
    enum class Type : unsigned { Button, HatUp, HatDown, HatLeft, HatRight, AxisLo, AxisHi } type;
    string name;
    string mapping;
    unsigned scancode;
    virtual void bind();
  };

  struct AnalogInput : AbstractInput {
  };

  struct DigitalInput : AbstractInput {
    int16_t poll();
  };

  struct Controller : array<AbstractInput*> {
    string name;
  };

  struct Gamepad : Controller {
    DigitalInput up, down, left, right;
    DigitalInput b, a, y, x;
    DigitalInput l, r, select, start;
    void create(const char *deviceName, const char *configName);
    int16_t poll(unsigned index);
  };

  struct Mouse : Controller {
    AnalogInput x, y;
    DigitalInput left, right;
    void create(const char *deviceName, const char *configName);
  };

  struct SuperScope : Controller {
    AnalogInput x, y;
    DigitalInput trigger, cursor, turbo, pause;
    void create(const char *deviceName, const char *configName);
  };

  struct Justifier : Controller {
    AnalogInput x, y;
    DigitalInput trigger, start;
    void create(const char *deviceName, const char *configName);
  };

  struct ControllerPort : array<Controller*> {
    string name;
  };

  struct Port1 : ControllerPort {
    Gamepad gamepad;
    Gamepad multitapA;
    Gamepad multitapB;
    Gamepad multitapC;
    Gamepad multitapD;
    Mouse mouse;
  } port1;

  struct Port2 : ControllerPort {
    Gamepad gamepad;
    Gamepad multitapA;
    Gamepad multitapB;
    Gamepad multitapC;
    Gamepad multitapD;
    Mouse mouse;
    SuperScope superScope;
    Justifier justifierA;
    Justifier justifierB;
  } port2;

  int16_t state[2][Scancode::Limit];
  bool activeState;

  void create();
  void bind();
  void poll();
  int16_t poll(bool port, SNES::Input::Device device, unsigned index, unsigned id);
  int16_t value(unsigned scancode);
};

extern InputMapper inputMapper;
