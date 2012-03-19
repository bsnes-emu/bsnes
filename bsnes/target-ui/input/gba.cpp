int16_t GbaController::poll(unsigned n) {
  switch((GBA::Input)n) {
  case GBA::Input::Up: return up.poll() & !down.poll();
  case GBA::Input::Down: return down.poll() & !up.poll();
  case GBA::Input::Left: return left.poll() & !right.poll();
  case GBA::Input::Right: return right.poll() & !left.poll();
  case GBA::Input::B: return b.poll() | bTurbo.poll();
  case GBA::Input::A: return a.poll() | aTurbo.poll();
  case GBA::Input::L: return l.poll() | lTurbo.poll();
  case GBA::Input::R: return r.poll() | rTurbo.poll();
  case GBA::Input::Select: return select.poll();
  case GBA::Input::Start: return start.poll();
  }

  return 0;  //never reached
}

GbaController::GbaController() {
  name = "Controller";

  up.name = "Up";
  down.name = "Down";
  left.name = "Left";
  right.name = "Right";
  b.name = "B";
  a.name = "A";
  l.name = "L";
  r.name = "R";
  select.name = "Select";
  start.name = "Start";

  bTurbo.name = "Turbo B";
  aTurbo.name = "Turbo A";
  lTurbo.name = "Turbo L";
  rTurbo.name = "Turbo R";

  up.mapping = "KB0::Up";
  down.mapping = "KB0::Down";
  left.mapping = "KB0::Left";
  right.mapping = "KB0::Right";
  b.mapping = "KB0::Z";
  a.mapping = "KB0::X";
  l.mapping = "KB0::A";
  r.mapping = "KB0::S";
  select.mapping = "KB0::Apostrophe";
  start.mapping = "KB0::Return";

  append(up, down, left, right, b, a, l, r, select, start);
  append(bTurbo, aTurbo, lTurbo, rTurbo);
}

//

GbaDevice::GbaDevice() {
  name = "Device";
  append(controller);
}

//

GbaInput::GbaInput() {
  name = "Game Boy Advance";
  append(device);
}
