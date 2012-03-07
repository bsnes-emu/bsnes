int16_t SnesGamepad::poll(unsigned n) {
  switch((SNES::Input::JoypadID)n) {
  case SNES::Input::JoypadID::Up: return up.poll() & !down.poll();
  case SNES::Input::JoypadID::Down: return down.poll() & !up.poll();
  case SNES::Input::JoypadID::Left: return left.poll() & !right.poll();
  case SNES::Input::JoypadID::Right: return right.poll() & !left.poll();
  case SNES::Input::JoypadID::B: return b.poll() | bTurbo.poll();
  case SNES::Input::JoypadID::A: return a.poll() | aTurbo.poll();
  case SNES::Input::JoypadID::Y: return y.poll() | yTurbo.poll();
  case SNES::Input::JoypadID::X: return x.poll() | xTurbo.poll();
  case SNES::Input::JoypadID::L: return l.poll() | lTurbo.poll();
  case SNES::Input::JoypadID::R: return r.poll() | rTurbo.poll();
  case SNES::Input::JoypadID::Select: return select.poll();
  case SNES::Input::JoypadID::Start: return start.poll();
  }
  return 0;
}

SnesGamepad::SnesGamepad(const string &name, bool defaultBindings) {
  this->name = name;

  up.name = "Up", down.name = "Down", left.name = "Left", right.name = "Right";
  b.name = "B", a.name = "A", y.name = "Y", x.name = "X";
  l.name = "L", r.name = "R", select.name = "Select", start.name = "Start";
  bTurbo.name = "Turbo B", aTurbo.name = "Turbo A", yTurbo.name = "Turbo Y", xTurbo.name = "Turbo X";
  lTurbo.name = "Turbo L", rTurbo.name = "Turbo R";

  if(defaultBindings) {
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

  append(up, down, left, right, b, a, y, x, l, r, select, start);
  append(bTurbo, aTurbo, yTurbo, xTurbo, lTurbo, rTurbo);
}

//

int16_t SnesMouse::poll(unsigned n) {
  switch((SNES::Input::MouseID)n) {
  case SNES::Input::MouseID::X: return xaxis.poll();
  case SNES::Input::MouseID::Y: return yaxis.poll();
  case SNES::Input::MouseID::Left: return left.poll();
  case SNES::Input::MouseID::Right: return right.poll();
  }
  return 0;
}

SnesMouse::SnesMouse(const string &name, bool defaultBindings) {
  this->name = name;

  xaxis.name = "X-axis", yaxis.name = "Y-axis";
  left.name = "Left button", right.name = "Right button";

  if(defaultBindings) {
    xaxis.mapping = "MS0::Xaxis";
    yaxis.mapping = "MS0::Yaxis";
    left.mapping = "MS0::Button0";
    right.mapping = "MS0::Button2";
  }

  append(xaxis, yaxis, left, right);
}

//

int16_t SnesSuperScope::poll(unsigned n) {
  switch((SNES::Input::SuperScopeID)n) {
  case SNES::Input::SuperScopeID::X: return xaxis.poll();
  case SNES::Input::SuperScopeID::Y: return yaxis.poll();
  case SNES::Input::SuperScopeID::Trigger: return trigger.poll();
  case SNES::Input::SuperScopeID::Cursor: return cursor.poll();
  case SNES::Input::SuperScopeID::Turbo: return turbo.poll();
  case SNES::Input::SuperScopeID::Pause: return pause.poll();
  }
  return 0;
}

SnesSuperScope::SnesSuperScope(const string &name, bool defaultBindings) {
  this->name = name;

  xaxis.name = "X-axis", yaxis.name = "Y-axis";
  trigger.name = "Trigger", cursor.name = "Cursor", turbo.name = "Turbo", pause.name = "Pause";

  if(defaultBindings) {
    xaxis.mapping = "MS0::Xaxis";
    yaxis.mapping = "MS0::Yaxis";
    trigger.mapping = "MS0::Button0";
    cursor.mapping = "MS0::Button2";
    turbo.mapping = "KB0::T";
    pause.mapping = "KB0::P";
  }

  append(xaxis, yaxis, trigger, cursor, turbo, pause);
}

//

int16_t SnesJustifier::poll(unsigned n) {
  switch((SNES::Input::JustifierID)n) {
  case SNES::Input::JustifierID::X: return xaxis.poll();
  case SNES::Input::JustifierID::Y: return yaxis.poll();
  case SNES::Input::JustifierID::Trigger: return trigger.poll();
  case SNES::Input::JustifierID::Start: return start.poll();
  }
  return 0;
}

SnesJustifier::SnesJustifier(const string &name, bool defaultBindings) {
  this->name = name;

  xaxis.name = "X-axis", yaxis.name = "Y-axis";
  trigger.name = "Trigger", start.name = "Start";

  if(defaultBindings) {
    xaxis.mapping = "MS0::Xaxis";
    yaxis.mapping = "MS0::Yaxis";
    trigger.mapping = "MS0::Button0";
    start.mapping = "MS0::Button2";
  }

  append(xaxis, yaxis, trigger, start);
}

//

SnesPort1Input::SnesPort1Input():
gamepad("Gamepad", true),
multitap1("Multitap - Port 1", false),
multitap2("Multitap - Port 2", false),
multitap3("Multitap - Port 3", false),
multitap4("Multitap - Port 4", false),
mouse("Mouse", true)
{
  name = "Controller Port 1";
  append(gamepad);
  append(multitap1);
  append(multitap2);
  append(multitap3);
  append(multitap4);
  append(mouse);
}

//

SnesPort2Input::SnesPort2Input():
gamepad("Gamepad", false),
multitap1("Multitap - Port 1", false),
multitap2("Multitap - Port 2", false),
multitap3("Multitap - Port 3", false),
multitap4("Multitap - Port 4", false),
mouse("Mouse", true),
superScope("Super Scope", true),
justifier1("Justifier - Port 1", true),
justifier2("Justifier - Port 2", false)
{
  name = "Controller Port 2";
  append(gamepad);
  append(multitap1);
  append(multitap2);
  append(multitap3);
  append(multitap4);
  append(mouse);
  append(superScope);
  append(justifier1);
  append(justifier2);
}

//

SnesInput::SnesInput() {
  name = "SNES";
  append(port1, port2);
}
