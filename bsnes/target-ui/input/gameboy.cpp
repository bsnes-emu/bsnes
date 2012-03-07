int16_t GameBoyController::poll(unsigned n) {
  switch(n) {
  case 0: return up.poll() & !down.poll();
  case 1: return down.poll() & !up.poll();
  case 2: return left.poll() & !right.poll();
  case 3: return right.poll() & !left.poll();
  case 4: return b.poll() | bTurbo.poll();
  case 5: return a.poll() | aTurbo.poll();
  case 6: return select.poll();
  case 7: return start.poll();
  }
  return 0;
}

GameBoyController::GameBoyController() {
  name = "Controller";

  up.name = "Up";
  down.name = "Down";
  left.name = "Left";
  right.name = "Right";
  b.name = "B";
  a.name = "A";
  select.name = "Select";
  start.name = "Start";
  bTurbo.name = "Turbo B";
  aTurbo.name = "Turbo A";

  up.mapping = "KB0::Up";
  down.mapping = "KB0::Down";
  left.mapping = "KB0::Left";
  right.mapping = "KB0::Right";
  b.mapping = "KB0::Z";
  a.mapping = "KB0::X";
  select.mapping = "KB0::Apostrophe";
  start.mapping = "KB0::Return";

  append(up, down, left, right, b, a, select, start, bTurbo, aTurbo);
}

//

GameBoyDevice::GameBoyDevice() {
  name = "Device";
  append(controller);
}

//

GameBoyInput::GameBoyInput() {
  name = "Game Boy";
  append(device);
}
