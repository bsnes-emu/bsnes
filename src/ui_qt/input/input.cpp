#include "device.cpp"
#include "userinterface.cpp"

//============
//InputManager
//============

void InputManager::bind() {
  inputPool.bind();
  inputUiPool.bind();
}

void InputManager::poll() {
  if(config.input.focusPolicy == Configuration::Input::FocusPolicyIgnoreInput
  && winMain->window->isActiveWindow() == false) {
    inputPool.clear();
  } else {
    inputPool.poll(stateTable[activeState]);
  }
}

void InputManager::clear() {
  inputPool.clear();
}

void InputManager::flush() {
  for(unsigned i = 0; i < nall::input_limit; i++) {
    stateTable[0][i] = 0;
    stateTable[1][i] = 0;
  }
}

int16_t InputManager::state(uint16_t code) {
  return stateTable[activeState][code];
}

int16_t InputManager::getStatus(unsigned deviceid, unsigned id) {
  InputDevice *device = inputPool.find((SNES::Input::DeviceID)deviceid);
  if(device) return device->state(id);
  return 0;
}

void InputManager::refresh() {
  bool last = activeState;
  activeState = !activeState;
  bool next = activeState;

  input.poll(stateTable[next]);
  for(unsigned i = 0; i < nall::input_limit; i++) {
    //call on_input() whenever button is pressed down; ignore axes
    if(!stateTable[last][i] && stateTable[next][i] && InputCode::isButton(i) && onInput) onInput(i);
  }
}

InputManager::InputManager() {
  activeState = 0;
  flush();
}

//=========
//InputCode
//=========

InputCode::type_t InputCode::type(uint16_t code) {
  if(code < keyboard::limit) return KeyboardButton;
  if(code >= mouse::x && code <= mouse::z) return MouseAxis;
  if(code < mouse::limit) return MouseButton;
  for(unsigned i = 0; i < joypad<>::count; i++) {
    unsigned index = joypad<>::index(i, joypad<>::axis);
    if(code >= index && code < index + joypad<>::axes) return JoypadAxis;
    if(code < joypad<>::index(i, joypad<>::limit)) return JoypadButton;
  }
  return Unknown;
}

bool InputCode::isButton(uint16_t code) {
  type_t n = type(code);
  return (n == KeyboardButton || n == MouseButton || n == JoypadButton);
}

bool InputCode::isAxis(uint16_t code) {
  type_t n = type(code);
  return (n == MouseAxis || n == JoypadAxis);
}

//===========
//InputObject
//===========

void InputObject::bind() {
  code = nall::input_find((const char*)id);
}

InputObject::InputObject(InputObject::type_t t, const char *n, string &s) : parent(0), type(t), name(n), id(s) {
}

//==========
//InputGroup
//==========

void InputGroup::attach(InputObject &object) {
  list.add(&object);
  object.parent = this;
}

void InputGroup::bind() {
  for(unsigned i = 0; i < list.size(); i++) list[i]->bind();
}

void InputGroup::clear() {
  for(unsigned i = 0; i < list.size(); i++) list[i]->state = 0;
}

void InputGroup::poll(const int16_t *table) {
  for(unsigned i = 0; i < list.size(); i++) {
    InputCode::type_t type = InputCode::type(list[i]->code);

    if(type == InputCode::MouseAxis && !input.acquired()) {
      //mouse must be acquired (locked to window) to move axes
      list[i]->state = 0;
    } else if(type == InputCode::MouseButton && !input.acquired()) {
      //same for buttons
      list[i]->state = 0;
    } else if(type == InputCode::JoypadAxis) {
      //joypad axis range = -32768 to +32767, scale to -8 to +7 to roughly match mouse delta
      list[i]->state = table[list[i]->code] / 4096;
    } else {
      list[i]->state = table[list[i]->code];
    }
  }
}

int16_t InputGroup::state(unsigned index) const {
  if(index < list.size()) return list[index]->state;
  return 0;
}

InputGroup::InputGroup(const char *n) : list(*this), name(n) {
}
