#include "../base.hpp"
#include "nes.cpp"
#include "snes.cpp"
#include "gameboy.cpp"
#include "user-interface.cpp"
InputManager *inputManager = 0;

void AbstractInput::attach(const string &primaryName, const string &secondaryName, const string &tertiaryName) {
  string name = { primaryName, "::", secondaryName, "::", tertiaryName, "::", this->name };
  name.replace(" ", "");
  inputManager->config.attach(mapping, name);
}

void AbstractInput::bind() {
       if(mapping.endswith(".Up")) type = Type::HatUp;
  else if(mapping.endswith(".Down")) type = Type::HatDown;
  else if(mapping.endswith(".Left")) type = Type::HatLeft;
  else if(mapping.endswith(".Right")) type = Type::HatRight;
  else if(mapping.endswith(".Lo")) type = Type::AxisLo;
  else if(mapping.endswith(".Hi")) type = Type::AxisHi;
  else if(mapping.beginswith("JP") && mapping.position("Axis")) type = Type::Axis;
  else if(mapping.beginswith("MS") && mapping.endswith("axis")) type = Type::Axis;
  else type = Type::Button;

  string decode = mapping;
  if(auto position = decode.position(".")) decode[position()] = 0;
  scancode = Scancode::decode(decode);
}

int16_t AbstractInput::poll() {
  return inputManager->scancode[inputManager->activeScancode][scancode];
}

//

bool AnalogInput::bind(int16_t scancode, int16_t value) {
  string encode = Scancode::encode(scancode);

  if(Mouse::isAnyAxis(scancode)) {
    for(unsigned n = 0; n < Mouse::Count; n++) {
      if(scancode == mouse(n)[Mouse::Xaxis]) { encode.append(".Xaxis"); goto bind; }
      if(scancode == mouse(n)[Mouse::Yaxis]) { encode.append(".Yaxis"); goto bind; }
      if(scancode == mouse(n)[Mouse::Zaxis]) { encode.append(".Zaxis"); goto bind; }
    }
  }

  if(Joypad::isAnyAxis(scancode)) {
    goto bind;
  }

  return false;

bind:
  mapping = encode;
  this->scancode = scancode;
  this->type = Type::Axis;
}

//

bool DigitalInput::bind(int16_t scancode, int16_t value) {
  string encode = Scancode::encode(scancode);
  Type type;

  if(Keyboard::isAnyKey(scancode) || Keyboard::isAnyModifier(scancode) || Joypad::isAnyButton(scancode)) {
    if(value == 0) return false;
    type = Type::Button;
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
  case Type::Button: return (bool)(value);
  case Type::HatUp: return (bool)(value & Joypad::HatUp);
  case Type::HatDown: return (bool)(value & Joypad::HatDown);
  case Type::HatLeft: return (bool)(value & Joypad::HatLeft);
  case Type::HatRight: return (bool)(value & Joypad::HatRight);
  case Type::AxisLo: return (bool)(value < -16384);
  case Type::AxisHi: return (bool)(value > +16384);
  }
  return 0;
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
      inputSettings->inputEvent(n, scancode[activeScancode][n]);
      userInterface.inputEvent(n, scancode[activeScancode][n]);
    }
  }
}

InputManager::InputManager() {
  inputManager = this;

  inputList.append(nes);
  inputList.append(snes);
  inputList.append(gameBoy);
  inputList.append(userInterface);

  for(unsigned n = 0; n < inputList.size(); n++) inputList[n].attach();
  config.load(string{ application->userpath, "input.cfg" });
  config.save(string{ application->userpath, "input.cfg" });
  for(unsigned n = 0; n < inputList.size(); n++) inputList[n].bind();

  activeScancode = 0;
}

InputManager::~InputManager() {
  config.save(string{ application->userpath, "input.cfg" });
}
