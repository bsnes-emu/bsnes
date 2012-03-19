#include "../base.hpp"
#include "nes.cpp"
#include "snes.cpp"
#include "gb.cpp"
#include "gba.cpp"
#include "user-interface.cpp"
InputManager *inputManager = nullptr;

void AbstractInput::attach(const string &primaryName, const string &secondaryName, const string &tertiaryName) {
  string name = { primaryName, "::", secondaryName, "::", tertiaryName, "::", this->name };
  name.replace(" ", "");
  inputManager->config.attach(mapping, name);
}

void AbstractInput::bind() {
  if(mapping == "") type = Type::Button, mapping = "None";

       if(mapping.endswith(".Up")) type = Type::HatUp;
  else if(mapping.endswith(".Down")) type = Type::HatDown;
  else if(mapping.endswith(".Left")) type = Type::HatLeft;
  else if(mapping.endswith(".Right")) type = Type::HatRight;
  else if(mapping.endswith(".Lo")) type = Type::AxisLo;
  else if(mapping.endswith(".Hi")) type = Type::AxisHi;
  else if(mapping.beginswith("JP") && mapping.position("Axis")) type = Type::Axis;
  else if(mapping.beginswith("MS") && mapping.endswith("axis")) type = Type::MouseAxis;
  else if(mapping.beginswith("MS")) type = Type::MouseButton;
  else type = Type::Button;

  string decode = mapping;
  if(auto position = decode.position(".")) decode[position()] = 0;
  scancode = Scancode::decode(decode);
}

int16_t AbstractInput::poll() {
  if(config->input.focusPolicy == 1 && mainWindow->focused() == false) return 0;
  return inputManager->scancode[inputManager->activeScancode][scancode];
}

//

bool AnalogInput::bind(int16_t scancode, int16_t value) {
  using nall::Mouse;
  string encode = Scancode::encode(scancode);
  Type type = Type::Button;

  if(scancode == Scancode::None) goto bind;
  if(Mouse::isAnyAxis(scancode)) { type = Type::MouseAxis; goto bind; }
  if(Joypad::isAnyAxis(scancode)) { type = Type::Axis; goto bind; }

  return false;

bind:
  mapping = encode;
  this->scancode = scancode;
  this->type = Type::Axis;
  return true;
}

int16_t AnalogInput::poll() {
  int16_t value = AbstractInput::poll();
  switch(type) {
  case Type::MouseAxis: return input.acquired() ? value : 0;
  case Type::Axis:      return value;
  }
  return 0;
}

//

bool DigitalInput::bind(int16_t scancode, int16_t value) {
  using nall::Keyboard;
  using nall::Mouse;
  string encode = Scancode::encode(scancode);
  Type type = Type::Button;

  if(scancode == Scancode::None) goto bind;

  if(Keyboard::isAnyKey(scancode) || Keyboard::isAnyModifier(scancode) || Joypad::isAnyButton(scancode)) {
    if(value == 0) return false;
    type = Type::Button;
    goto bind;
  }

  if(Mouse::isAnyButton(scancode)) {
    if(value == 0) return false;
    type = Type::MouseButton;
    goto bind;
  }

  if(Joypad::isAnyHat(scancode)) {
    if(value & Joypad::HatUp   ) { type = Type::HatUp;    encode.append(".Up"   ); goto bind; }
    if(value & Joypad::HatDown ) { type = Type::HatDown;  encode.append(".Down" ); goto bind; }
    if(value & Joypad::HatLeft ) { type = Type::HatLeft;  encode.append(".Left" ); goto bind; }
    if(value & Joypad::HatRight) { type = Type::HatRight; encode.append(".Right"); goto bind; }
  }

  if(Joypad::isAnyAxis(scancode)) {
    if(value < -12288) { type = Type::AxisLo; encode.append(".Lo"); goto bind; }
    if(value > +24576) { type = Type::AxisHi; encode.append(".Hi"); goto bind; }
  }

  return false;

bind:
  mapping = encode;
  this->scancode = scancode;
  this->type = type;
  return true;
}

int16_t DigitalInput::poll() {
  int16_t value = AbstractInput::poll();
  switch(type) {
  case Type::Button:      return (bool)(value);
  case Type::MouseButton: return (bool)(value & input.acquired());
  case Type::HatUp:       return (bool)(value & Joypad::HatUp);
  case Type::HatDown:     return (bool)(value & Joypad::HatDown);
  case Type::HatLeft:     return (bool)(value & Joypad::HatLeft);
  case Type::HatRight:    return (bool)(value & Joypad::HatRight);
  case Type::AxisLo:      return (bool)(value < -16384);
  case Type::AxisHi:      return (bool)(value > +16384);
  }
  return 0;
}

//

int16_t TurboInput::poll() {
  int16_t result = DigitalInput::poll();
  if(phase < 3) result = 0;
  if(++phase >= 6) phase = 0;
  return result;
}

TurboInput::TurboInput() : phase(0) {
}

//

void TertiaryInput::attach(const string &primaryName, const string &secondaryName) {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).attach(primaryName, secondaryName, name);
  }
}

void TertiaryInput::bind() {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).bind();
  }
}

int16_t TertiaryInput::poll(unsigned n) {
  return operator[](n).poll();
}

//

void SecondaryInput::attach(const string &primaryName) {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).attach(primaryName, name);
  }
}

void SecondaryInput::bind() {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).bind();
  }
}

//

void PrimaryInput::attach() {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).attach(name);
  }
}

void PrimaryInput::bind() {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).bind();
  }
}

//

void InputManager::scan() {
  activeScancode = !activeScancode;
  input.poll(scancode[activeScancode]);

  for(unsigned n = 0; n < Scancode::Limit; n++) {
    if(scancode[!activeScancode][n] != scancode[activeScancode][n]) {
      if(settingsWindow->focused()) inputSettings->inputEvent(n, scancode[activeScancode][n]);
      if(mainWindow->focused()) userInterface.inputEvent(n, scancode[activeScancode][n]);
    }
  }
}

InputManager::InputManager() {
  inputManager = this;

  inputList.append(nes);
  inputList.append(snes);
  inputList.append(gb);
  inputList.append(gba);
  inputList.append(userInterface);

  for(unsigned n = 0; n < inputList.size(); n++) inputList[n].attach();
  config.load(application->path("input.cfg"));
  config.save(application->path("input.cfg"));
  for(unsigned n = 0; n < inputList.size(); n++) inputList[n].bind();

  activeScancode = 0;
}

InputManager::~InputManager() {
  config.save(application->path("input.cfg"));
}
