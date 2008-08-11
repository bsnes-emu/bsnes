class InputManager {
public:
  //  0 = Joypad 1
  //  1 = Joypad 2
  //2-5 = Multitap 1
  //6-9 = Multitap 2
  struct Joypad {
    struct Button {
      uint16_t value;
      bool state;
    } up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad[10];

  struct GUI {
    uint16_t load;
    uint16_t pause;
    uint16_t reset;
    uint16_t power;
    uint16_t quit;
    uint16_t speed_decrease;
    uint16_t speed_increase;
    uint16_t frameskip_decrease;
    uint16_t frameskip_increase;
    uint16_t toggle_fullscreen;
    uint16_t toggle_menubar;
    uint16_t toggle_statusbar;
  } gui;

  void bind();
  void poll();
  bool get_status(unsigned deviceid, unsigned id);

  void refresh();
  function<void (uint16_t)> on_keydown;
  function<void (uint16_t)> on_keyup;

  InputManager();
  ~InputManager();

private:
  uint8_t *state;
} input_manager;

//refresh input state for PC keyboard and joypads
//callbacks can be bound to on_keydown and on_keyup
//callbacks will be invoked whenever input status changes
//this should be called roughly every ~20-50ms
//however, this need not be called if no callbacks are attached
void InputManager::refresh() {
  input.poll();
  for(uint i = 0; i < input_limit; i++) {
    bool prev_state = state[i];
    state[i] = input.key_down(i);
    if(!prev_state &&  state[i] && on_keydown) on_keydown(i);
    if( prev_state && !state[i] && on_keyup  ) on_keyup(i);
  }
}

void InputManager::bind() {
  #define map(i, n) \
    joypad[i].up.value     = input_find(config::input.n.up); \
    joypad[i].down.value   = input_find(config::input.n.down); \
    joypad[i].left.value   = input_find(config::input.n.left); \
    joypad[i].right.value  = input_find(config::input.n.right); \
    joypad[i].a.value      = input_find(config::input.n.a); \
    joypad[i].b.value      = input_find(config::input.n.b); \
    joypad[i].x.value      = input_find(config::input.n.x); \
    joypad[i].y.value      = input_find(config::input.n.y); \
    joypad[i].l.value      = input_find(config::input.n.l); \
    joypad[i].r.value      = input_find(config::input.n.r); \
    joypad[i].select.value = input_find(config::input.n.select); \
    joypad[i].start.value  = input_find(config::input.n.start);

  map(0, joypad1)
  map(1, joypad2)
  map(2, multitap1a)
  map(3, multitap1b)
  map(4, multitap1c)
  map(5, multitap1d)
  map(6, multitap2a)
  map(7, multitap2b)
  map(8, multitap2c)
  map(9, multitap2d)

  #undef map

  for(unsigned i = 0; i < 10; i++) {
    joypad[i].up.state = joypad[i].down.state = joypad[i].left.state   = joypad[i].right.state =
    joypad[i].a.state  = joypad[i].b.state    = joypad[i].x.state      = joypad[i].y.state     =
    joypad[i].l.state  = joypad[i].r.state    = joypad[i].select.state = joypad[i].start.state =
    false;
  }

  gui.load               = input_find(config::input.gui.load);
  gui.pause              = input_find(config::input.gui.pause);
  gui.reset              = input_find(config::input.gui.reset);
  gui.power              = input_find(config::input.gui.power);
  gui.quit               = input_find(config::input.gui.quit);
  gui.speed_decrease     = input_find(config::input.gui.speed_decrease);
  gui.speed_increase     = input_find(config::input.gui.speed_increase);
  gui.frameskip_decrease = input_find(config::input.gui.frameskip_decrease);
  gui.frameskip_increase = input_find(config::input.gui.frameskip_increase);
  gui.toggle_fullscreen  = input_find(config::input.gui.toggle_fullscreen);
  gui.toggle_menubar     = input_find(config::input.gui.toggle_menubar);
  gui.toggle_statusbar   = input_find(config::input.gui.toggle_statusbar);
}

void InputManager::poll() {
  for(unsigned i = 0; i < 10; i++) {
    joypad[i].up.state     = input.key_down(joypad[i].up.value);
    joypad[i].down.state   = input.key_down(joypad[i].down.value);
    joypad[i].left.state   = input.key_down(joypad[i].left.value);
    joypad[i].right.state  = input.key_down(joypad[i].right.value);
    joypad[i].a.state      = input.key_down(joypad[i].a.value);
    joypad[i].b.state      = input.key_down(joypad[i].b.value);
    joypad[i].x.state      = input.key_down(joypad[i].x.value);
    joypad[i].y.state      = input.key_down(joypad[i].y.value);
    joypad[i].l.state      = input.key_down(joypad[i].l.value);
    joypad[i].r.state      = input.key_down(joypad[i].r.value);
    joypad[i].select.state = input.key_down(joypad[i].select.value);
    joypad[i].start.state  = input.key_down(joypad[i].start.value);
  }
}

bool InputManager::get_status(unsigned deviceid, unsigned id) {
  //=======
  //Joypads
  //=======
  int index = -1;
  switch(deviceid) {
    case SNES::Input::DeviceIDJoypad1:    index = 0; break;
    case SNES::Input::DeviceIDJoypad2:    index = 1; break;
    case SNES::Input::DeviceIDMultitap1A: index = 2; break;
    case SNES::Input::DeviceIDMultitap1B: index = 3; break;
    case SNES::Input::DeviceIDMultitap1C: index = 4; break;
    case SNES::Input::DeviceIDMultitap1D: index = 5; break;
    case SNES::Input::DeviceIDMultitap2A: index = 6; break;
    case SNES::Input::DeviceIDMultitap2B: index = 7; break;
    case SNES::Input::DeviceIDMultitap2C: index = 8; break;
    case SNES::Input::DeviceIDMultitap2D: index = 9; break;
  }

  if(index >= 0) {
    switch(id) {
      case SNES::Input::JoypadUp:     return joypad[index].up.state;
      case SNES::Input::JoypadDown:   return joypad[index].down.state;
      case SNES::Input::JoypadLeft:   return joypad[index].left.state;
      case SNES::Input::JoypadRight:  return joypad[index].right.state;
      case SNES::Input::JoypadA:      return joypad[index].a.state;
      case SNES::Input::JoypadB:      return joypad[index].b.state;
      case SNES::Input::JoypadX:      return joypad[index].x.state;
      case SNES::Input::JoypadY:      return joypad[index].y.state;
      case SNES::Input::JoypadL:      return joypad[index].l.state;
      case SNES::Input::JoypadR:      return joypad[index].r.state;
      case SNES::Input::JoypadSelect: return joypad[index].select.state;
      case SNES::Input::JoypadStart:  return joypad[index].start.state;
    }
  }

  return false;
}

InputManager::InputManager() {
  state = new(zeromemory) uint8_t[input_limit];
}

InputManager::~InputManager() {
  delete[] state;
}
