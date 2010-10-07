#include "../base.hpp"
#include "hotkeys.cpp"
InputMapper inputMapper;

void InputMapper::AbstractInput::bind() {
  if(strend(mapping, ".Up")) type = Type::HatUp;
  else if(strend(mapping, ".Down")) type = Type::HatDown;
  else if(strend(mapping, ".Left")) type = Type::HatLeft;
  else if(strend(mapping, ".Right")) type = Type::HatRight;
  else if(strend(mapping, ".Lo")) type = Type::AxisLo;
  else if(strend(mapping, ".Hi")) type = Type::AxisHi;
  else if(strbegin(mapping, "MS") && strend(mapping, "axis")) type = Type::MouseAxis;
  else if(strbegin(mapping, "MS")) type = Type::MouseButton;
  else type = Type::Button;

  string mappingValue = mapping;
  if(auto position = strpos(mappingValue, ".")) mappingValue[position()] = 0;
  scancode = Scancode::decode(mappingValue);
}

int16_t InputMapper::AnalogInput::poll() {
  int16_t value = inputMapper.state[inputMapper.activeState][scancode];
  switch(type) {
    case AbstractInput::Type::MouseAxis: {
      if(input.acquired() == false) return 0;
      return value;
    }
  }
  return 0;
}

int16_t InputMapper::DigitalInput::poll() {
  int16_t value = inputMapper.state[inputMapper.activeState][scancode];
  switch(type) {
    case AbstractInput::Type::Button: return (bool)value;
    case AbstractInput::Type::MouseButton: return (bool)value & input.acquired();
    case AbstractInput::Type::HatUp: return (bool)(value & Joypad::HatUp);
    case AbstractInput::Type::HatDown: return (bool)(value & Joypad::HatDown);
    case AbstractInput::Type::HatLeft: return (bool)(value & Joypad::HatLeft);
    case AbstractInput::Type::HatRight: return (bool)(value & Joypad::HatRight);
    case AbstractInput::Type::AxisLo: return (bool)(value < -16384);
    case AbstractInput::Type::AxisHi: return (bool)(value > +16384);
  }
}

void InputMapper::Gamepad::create(const char *deviceName, const char *configName) {
  name = deviceName;
  up.name = "Up"; down.name = "Down"; left.name = "Left"; right.name = "Right";
  b.name = "B"; a.name = "A"; y.name = "Y"; x.name = "X";
  l.name = "L"; r.name = "R"; select.name = "Select"; start.name = "Start";
  append(&up); append(&down); append(&left); append(&right);
  append(&b); append(&a); append(&y); append(&x);
  append(&l); append(&r); append(&select); append(&start);

  config.attach(up.mapping = "", string("input.", configName, ".up"));
  config.attach(down.mapping = "", string("input.", configName, ".down"));
  config.attach(left.mapping = "", string("input.", configName, ".left"));
  config.attach(right.mapping = "", string("input.", configName, ".right"));
  config.attach(b.mapping = "", string("input.", configName, ".b"));
  config.attach(a.mapping = "", string("input.", configName, ".a"));
  config.attach(y.mapping = "", string("input.", configName, ".y"));
  config.attach(x.mapping = "", string("input.", configName, ".x"));
  config.attach(l.mapping = "", string("input.", configName, ".l"));
  config.attach(r.mapping = "", string("input.", configName, ".r"));
  config.attach(select.mapping = "", string("input.", configName, ".select"));
  config.attach(start.mapping = "", string("input.", configName, ".start"));

  if(!strcmp(configName, "port1.gamepad")) {
    up.mapping = "KB0::Up";
    down.mapping = "KB0::Down";
    left.mapping = "KB0::Left";
    right.mapping = "KB0::Right";
    b.mapping = "KB0::Z";
    a.mapping = "KB0::X";
    y.mapping = "KB0::A";
    x.mapping = "KB0::S";
    l.mapping = "KB0::D";
    r.mapping = "KB0::C";
    select.mapping = "KB0::Apostrophe";
    start.mapping = "KB0::Return";
  }
}

int16_t InputMapper::Gamepad::poll(unsigned id) {
  switch(id) {
    case SNES::Input::JoypadID::Up:     return up.poll();
    case SNES::Input::JoypadID::Down:   return down.poll() & !up.poll();
    case SNES::Input::JoypadID::Left:   return left.poll();
    case SNES::Input::JoypadID::Right:  return right.poll() & !left.poll();
    case SNES::Input::JoypadID::B:      return b.poll();
    case SNES::Input::JoypadID::A:      return a.poll();
    case SNES::Input::JoypadID::Y:      return y.poll();
    case SNES::Input::JoypadID::X:      return x.poll();
    case SNES::Input::JoypadID::L:      return l.poll();
    case SNES::Input::JoypadID::R:      return r.poll();
    case SNES::Input::JoypadID::Select: return select.poll();
    case SNES::Input::JoypadID::Start:  return start.poll();
  }
  return 0;
}

void InputMapper::Mouse::create(const char *deviceName, const char *configName) {
  name = deviceName;
  x.name = "X-axis"; y.name = "Y-axis";
  left.name = "Left Button"; right.name = "Right Button";
  append(&x); append(&y);
  append(&left); append(&right);

  config.attach(x.mapping = "", string("input.", configName, ".x"));
  config.attach(y.mapping = "", string("input.", configName, ".y"));
  config.attach(left.mapping = "", string("input.", configName, ".left"));
  config.attach(right.mapping = "", string("input.", configName, ".right"));

  x.mapping = "MS0::Xaxis";
  y.mapping = "MS0::Yaxis";
  left.mapping = "MS0::Button0";
  right.mapping = "MS0::Button2";
}

int16_t InputMapper::Mouse::poll(unsigned id) {
  switch(id) {
    case SNES::Input::MouseID::X: return x.poll();
    case SNES::Input::MouseID::Y: return y.poll();
    case SNES::Input::MouseID::Left: return left.poll();
    case SNES::Input::MouseID::Right: return right.poll();
  }
  return 0;
}

void InputMapper::SuperScope::create(const char *deviceName, const char *configName) {
  name = deviceName;
  x.name = "X-axis"; y.name = "Y-axis";
  trigger.name = "Trigger"; cursor.name = "Cursor"; turbo.name = "Turbo"; pause.name = "Pause";
  append(&x); append(&y);
  append(&trigger); append(&cursor); append(&turbo); append(&pause);

  config.attach(x.mapping = "", string("input.", configName, ".x"));
  config.attach(y.mapping = "", string("input.", configName, ".y"));
  config.attach(trigger.mapping = "", string("input.", configName, ".trigger"));
  config.attach(cursor.mapping = "", string("input.", configName, ".cursor"));
  config.attach(turbo.mapping = "", string("input.", configName, ".turbo"));
  config.attach(pause.mapping = "", string("input.", configName, ".pause"));

  x.mapping = "MS0::Xaxis";
  y.mapping = "MS0::Yaxis";
  trigger.mapping = "MS0::Button0";
  cursor.mapping = "MS0::Button2";
  turbo.mapping = "KB0::T";
  pause.mapping = "KB0::P";
}

int16_t InputMapper::SuperScope::poll(unsigned id) {
  switch(id) {
    case SNES::Input::SuperScopeID::X: return x.poll();
    case SNES::Input::SuperScopeID::Y: return y.poll();
    case SNES::Input::SuperScopeID::Trigger: return trigger.poll();
    case SNES::Input::SuperScopeID::Cursor: return cursor.poll();
    case SNES::Input::SuperScopeID::Turbo: return turbo.poll();
    case SNES::Input::SuperScopeID::Pause: return pause.poll();
  }
  return 0;
}

void InputMapper::Justifier::create(const char *deviceName, const char *configName) {
  name = deviceName;
  x.name = "X-axis"; y.name = "Y-axis";
  trigger.name = "Trigger"; start.name = "Start";
  append(&x); append(&y);
  append(&trigger); append(&start);

  config.attach(x.mapping = "", string("input.", configName, ".x"));
  config.attach(y.mapping = "", string("input.", configName, ".y"));
  config.attach(trigger.mapping = "", string("input.", configName, ".trigger"));
  config.attach(start.mapping = "", string("input.", configName, ".start"));

  if(!strcmp(configName, "port2.justifierA")) {
    x.mapping = "MS0::Xaxis";
    y.mapping = "MS0::Yaxis";
    trigger.mapping = "MS0::Button0";
    start.mapping = "MS0::Button2";
  }
}

int16_t InputMapper::Justifier::poll(unsigned id) {
  switch(id) {
    case SNES::Input::JustifierID::X: return x.poll();
    case SNES::Input::JustifierID::Y: return y.poll();
    case SNES::Input::JustifierID::Trigger: return trigger.poll();
    case SNES::Input::JustifierID::Start: return start.poll();
  }
  return 0;
}

void InputMapper::create() {
  activeState = 0;

  port1.name = "Controller Port 1";
  port1.gamepad.create("Gamepad", "port1.gamepad");
  port1.multitapA.create("Multitap - Port 1", "port1.multitapA");
  port1.multitapB.create("Multitap - Port 2", "port1.multitapB");
  port1.multitapC.create("Multitap - Port 3", "port1.multitapC");
  port1.multitapD.create("Multitap - Port 4", "port1.multitapD");
  port1.mouse.create("Mouse", "port1.mouse");
  port1.append(&port1.gamepad);
  port1.append(&port1.multitapA);
  port1.append(&port1.multitapB);
  port1.append(&port1.multitapC);
  port1.append(&port1.multitapD);
  port1.append(&port1.mouse);

  port2.name = "Controller Port 2";
  port2.gamepad.create("Gamepad", "port2.gamepad");
  port2.multitapA.create("Multitap - Port 1", "port2.multitapA");
  port2.multitapB.create("Multitap - Port 2", "port2.multitapB");
  port2.multitapC.create("Multitap - Port 3", "port2.multitapC");
  port2.multitapD.create("Multitap - Port 4", "port2.multitapD");
  port2.mouse.create("Mouse", "port2.mouse");
  port2.superScope.create("Super Scope", "port2.superScope");
  port2.justifierA.create("Justifier - Port 1", "port2.justifierA");
  port2.justifierB.create("Justifier - Port 2", "port2.justifierB");
  port2.append(&port2.gamepad);
  port2.append(&port2.multitapA);
  port2.append(&port2.multitapB);
  port2.append(&port2.multitapC);
  port2.append(&port2.multitapD);
  port2.append(&port2.mouse);
  port2.append(&port2.superScope);
  port2.append(&port2.justifierA);
  port2.append(&port2.justifierB);
}

void InputMapper::bind() {
  for(unsigned i = 0; i < port1.size(); i++) {
    Controller &controller = *port1[i];
    for(unsigned n = 0; n < controller.size(); n++) controller[n]->bind();
  }
  for(unsigned i = 0; i < port2.size(); i++) {
    Controller &controller = *port2[i];
    for(unsigned n = 0; n < controller.size(); n++) controller[n]->bind();
  }
}

void InputMapper::poll() {
  activeState ^= 1;
  input.poll(state[activeState]);

  for(unsigned i = 0; i < Scancode::Limit; i++) {
    if(state[0][i] != state[1][i]) {
      poll_hotkeys(i, state[activeState][i]);
      inputSettings.inputEvent(i, state[activeState][i]);
    }
  }
}

int16_t InputMapper::poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0) {
    if(device == SNES::Input::Device::Joypad) return port1.gamepad.poll(id);
    if(device == SNES::Input::Device::Multitap) switch(index) {
      case 0: return port1.multitapA.poll(id);
      case 1: return port1.multitapB.poll(id);
      case 2: return port1.multitapC.poll(id);
      case 3: return port1.multitapD.poll(id);
    }
    if(device == SNES::Input::Device::Mouse) return port1.mouse.poll(id);
  } else {
    if(device == SNES::Input::Device::Joypad) return port2.gamepad.poll(id);
    if(device == SNES::Input::Device::Multitap) switch(index) {
      case 0: return port2.multitapA.poll(id);
      case 1: return port2.multitapB.poll(id);
      case 2: return port2.multitapC.poll(id);
      case 3: return port2.multitapD.poll(id);
    }
    if(device == SNES::Input::Device::Mouse) return port2.mouse.poll(id);
    if(device == SNES::Input::Device::SuperScope) return port2.superScope.poll(id);
    if(device == SNES::Input::Device::Justifier) return port2.justifierA.poll(id);
    if(device == SNES::Input::Device::Justifiers) switch(index) {
      case 0: return port2.justifierA.poll(id);
      case 1: return port2.justifierB.poll(id);
    }
  }
  return 0;
}

int16_t InputMapper::value(unsigned scancode) {
  return state[activeState][scancode];
}
