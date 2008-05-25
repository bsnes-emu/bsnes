class InputManager {
public:
  struct Joypad {
    struct Button {
      uint16_t value;
      bool state;
    } up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1, joypad2;

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
  bool get_status(uint device, uint button);

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
  joypad1.up.value     = input_find(config::input.joypad1.up);
  joypad1.down.value   = input_find(config::input.joypad1.down);
  joypad1.left.value   = input_find(config::input.joypad1.left);
  joypad1.right.value  = input_find(config::input.joypad1.right);
  joypad1.a.value      = input_find(config::input.joypad1.a);
  joypad1.b.value      = input_find(config::input.joypad1.b);
  joypad1.x.value      = input_find(config::input.joypad1.x);
  joypad1.y.value      = input_find(config::input.joypad1.y);
  joypad1.l.value      = input_find(config::input.joypad1.l);
  joypad1.r.value      = input_find(config::input.joypad1.r);
  joypad1.select.value = input_find(config::input.joypad1.select);
  joypad1.start.value  = input_find(config::input.joypad1.start);

  joypad1.up.state = joypad1.down.state = joypad1.left.state   = joypad1.right.state =
  joypad1.a.state  = joypad1.b.state    = joypad1.x.state      = joypad1.y.state     =
  joypad1.l.state  = joypad1.r.state    = joypad1.select.state = joypad1.start.state =
  false;

  joypad2.up.value     = input_find(config::input.joypad2.up);
  joypad2.down.value   = input_find(config::input.joypad2.down);
  joypad2.left.value   = input_find(config::input.joypad2.left);
  joypad2.right.value  = input_find(config::input.joypad2.right);
  joypad2.a.value      = input_find(config::input.joypad2.a);
  joypad2.b.value      = input_find(config::input.joypad2.b);
  joypad2.x.value      = input_find(config::input.joypad2.x);
  joypad2.y.value      = input_find(config::input.joypad2.y);
  joypad2.l.value      = input_find(config::input.joypad2.l);
  joypad2.r.value      = input_find(config::input.joypad2.r);
  joypad2.select.value = input_find(config::input.joypad2.select);
  joypad2.start.value  = input_find(config::input.joypad2.start);

  joypad2.up.state = joypad2.down.state = joypad2.left.state   = joypad2.right.state =
  joypad2.a.state  = joypad2.b.state    = joypad2.x.state      = joypad2.y.state     =
  joypad2.l.state  = joypad2.r.state    = joypad2.select.state = joypad2.start.state =
  false;

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
  joypad1.up.state     = input.key_down(joypad1.up.value);
  joypad1.down.state   = input.key_down(joypad1.down.value);
  joypad1.left.state   = input.key_down(joypad1.left.value);
  joypad1.right.state  = input.key_down(joypad1.right.value);
  joypad1.a.state      = input.key_down(joypad1.a.value);
  joypad1.b.state      = input.key_down(joypad1.b.value);
  joypad1.x.state      = input.key_down(joypad1.x.value);
  joypad1.y.state      = input.key_down(joypad1.y.value);
  joypad1.l.state      = input.key_down(joypad1.l.value);
  joypad1.r.state      = input.key_down(joypad1.r.value);
  joypad1.select.state = input.key_down(joypad1.select.value);
  joypad1.start.state  = input.key_down(joypad1.start.value);

  joypad2.up.state     = input.key_down(joypad2.up.value);
  joypad2.down.state   = input.key_down(joypad2.down.value);
  joypad2.left.state   = input.key_down(joypad2.left.value);
  joypad2.right.state  = input.key_down(joypad2.right.value);
  joypad2.a.state      = input.key_down(joypad2.a.value);
  joypad2.b.state      = input.key_down(joypad2.b.value);
  joypad2.x.state      = input.key_down(joypad2.x.value);
  joypad2.y.state      = input.key_down(joypad2.y.value);
  joypad2.l.state      = input.key_down(joypad2.l.value);
  joypad2.r.state      = input.key_down(joypad2.r.value);
  joypad2.select.state = input.key_down(joypad2.select.value);
  joypad2.start.state  = input.key_down(joypad2.start.value);
}

bool InputManager::get_status(uint device, uint button) {
  switch(device) {
    case SNES::Input::DeviceIDJoypad1: {
      switch(button) {
        case SNES::Input::JoypadUp:     return joypad1.up.state;
        case SNES::Input::JoypadDown:   return joypad1.down.state;
        case SNES::Input::JoypadLeft:   return joypad1.left.state;
        case SNES::Input::JoypadRight:  return joypad1.right.state;
        case SNES::Input::JoypadA:      return joypad1.a.state;
        case SNES::Input::JoypadB:      return joypad1.b.state;
        case SNES::Input::JoypadX:      return joypad1.x.state;
        case SNES::Input::JoypadY:      return joypad1.y.state;
        case SNES::Input::JoypadL:      return joypad1.l.state;
        case SNES::Input::JoypadR:      return joypad1.r.state;
        case SNES::Input::JoypadSelect: return joypad1.select.state;
        case SNES::Input::JoypadStart:  return joypad1.start.state;
      }
    } break;

    case SNES::Input::DeviceIDJoypad2: {
      switch(button) {
        case SNES::Input::JoypadUp:     return joypad2.up.state;
        case SNES::Input::JoypadDown:   return joypad2.down.state;
        case SNES::Input::JoypadLeft:   return joypad2.left.state;
        case SNES::Input::JoypadRight:  return joypad2.right.state;
        case SNES::Input::JoypadA:      return joypad2.a.state;
        case SNES::Input::JoypadB:      return joypad2.b.state;
        case SNES::Input::JoypadX:      return joypad2.x.state;
        case SNES::Input::JoypadY:      return joypad2.y.state;
        case SNES::Input::JoypadL:      return joypad2.l.state;
        case SNES::Input::JoypadR:      return joypad2.r.state;
        case SNES::Input::JoypadSelect: return joypad2.select.state;
        case SNES::Input::JoypadStart:  return joypad2.start.state;
      }
    } break;
  }

  return false;
}

InputManager::InputManager() {
  state = new(zeromemory) uint8_t[input_limit];
}

InputManager::~InputManager() {
  delete[] state;
}
