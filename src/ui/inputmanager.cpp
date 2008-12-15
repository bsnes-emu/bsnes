#include "inputdevices.cpp"
#include "inputui.cpp"

class InputManager {
public:
  void bind();
  void poll();
  void clear();
  void flush();

  int16_t state(uint16_t code);
  int16_t get_status(unsigned deviceid, unsigned id);

  void refresh();
  function<void (uint16_t)> on_input;

  InputManager();
  ~InputManager();

private:
  bool active_state;
  int16_t state_table[2][input_limit];
} input_manager;

//refresh input state for PC keyboard and joypads
//callback can be bound to on_input
//this should be called roughly every ~20-50ms
//however, this need not be called if no callback is attached
void InputManager::refresh() {
  bool last = active_state;
  active_state = !active_state;
  bool next = active_state;

  input.poll(state_table[next]);
  for(unsigned i = 0; i < input_limit; i++) {
    //call on_input() whenever button is pressed down; ignores axes
    if(!state_table[last][i]
    && state_table[next][i]
    && InputCode::is_button(i)
    && on_input
    ) on_input(i);

    //detect any change of state for UI input capture window
    if(state_table[last][i] != state_table[next][i]
    && window_input_capture.focused()
    && window_input_capture.waiting
    && !window_input_capture.locked
    ) {
      //buttons only map upon pressing down; axes map on movement in any direction
      if(InputCode::is_button(i) == false || state_table[next][i]) {
        window_input_capture.assign(i);
      }
    }
  }

  //input capture locks to avoid immediate assignment of enter / spacebar / etc;
  //in other words, inputs that can be used to trigger the input capture window.
  //this will release the lock when none of said buttons are active.
  if(window_input_capture.focused()
  && window_input_capture.waiting
  && window_input_capture.locked
  ) {
    window_input_capture.locked
    =  input_manager.state(keyboard::return_)
    || input_manager.state(keyboard::spacebar)
    || input_manager.state(mouse::button + 0);
  }
}

void InputManager::bind() {
  inputpool.bind();
  inputuigeneral.bind();
  inputuidebugger.bind();
}

void InputManager::poll() {
  inputpool.poll(state_table[active_state]);
}

void InputManager::clear() {
  inputpool.clear();
}

void InputManager::flush() {
  for(unsigned i = 0; i < input_limit; i++) {
    state_table[0][i] = 0;
    state_table[1][i] = 0;
  }
}

int16_t InputManager::state(uint16_t code) {
  return state_table[active_state][code];
}

int16_t InputManager::get_status(unsigned deviceid, unsigned id) {
  InputDevice *device = inputpool.find((SNES::Input::DeviceID)deviceid);
  if(device) return device->state(id);
  return 0;
}

InputManager::InputManager() {
  active_state = 0;
  flush();
}

InputManager::~InputManager() {
}
