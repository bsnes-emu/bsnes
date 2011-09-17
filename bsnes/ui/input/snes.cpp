int16_t SnesGamepad::poll(unsigned n) {
  switch((SNES::Input::JoypadID)n) {
  case SNES::Input::JoypadID::Up: return up.poll() & !down.poll();
  case SNES::Input::JoypadID::Down: return down.poll() & !up.poll();
  case SNES::Input::JoypadID::Left: return left.poll() & !right.poll();
  case SNES::Input::JoypadID::Right: return right.poll() & !left.poll();
  case SNES::Input::JoypadID::B: return b.poll();
  case SNES::Input::JoypadID::A: return a.poll();
  case SNES::Input::JoypadID::Y: return y.poll();
  case SNES::Input::JoypadID::X: return x.poll();
  case SNES::Input::JoypadID::L: return l.poll();
  case SNES::Input::JoypadID::R: return r.poll();
  case SNES::Input::JoypadID::Select: return select.poll();
  case SNES::Input::JoypadID::Start: return start.poll();
  }
  return 0;
}

SnesGamepad::SnesGamepad() {
  name = "Gamepad";

  up.name = "Up";
  down.name = "Down";
  left.name = "Left";
  right.name = "Right";
  b.name = "B";
  a.name = "A";
  y.name = "Y";
  x.name = "X";
  l.name = "L";
  r.name = "R";
  select.name = "Select";
  start.name = "Start";

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

  append(up); append(down); append(left); append(right);
  append(b); append(a); append(y); append(x);
  append(l); append(r); append(select); append(start);
}

//

SnesPort1Input::SnesPort1Input() {
  name = "Controller Port 1";
  append(gamepad);
}

//

SnesPort2Input::SnesPort2Input() {
  name = "Controller Port 2";
  append(gamepad);
}

//

SnesInput::SnesInput() {
  name = "SNES";
  append(port1);
  append(port2);
}
