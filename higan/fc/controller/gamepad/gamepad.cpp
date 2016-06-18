Gamepad::Gamepad(bool port) : Controller(port) {
}

auto Gamepad::data() -> uint3 {
  if(counter >= 8) return 1;
  if(latched == 1) return interface->inputPoll(port, Device::Gamepad, A);

  switch(counter++) {
  case 0: return a;
  case 1: return b;
  case 2: return select;
  case 3: return start;
  case 4: return up && !down;
  case 5: return down && !up;
  case 6: return left && !right;
  case 7: return right && !left;
  }
  unreachable;
}

auto Gamepad::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    auto id = Device::Gamepad;
    a      = interface->inputPoll(port, id, A);
    b      = interface->inputPoll(port, id, B);
    select = interface->inputPoll(port, id, Select);
    start  = interface->inputPoll(port, id, Start);
    up     = interface->inputPoll(port, id, Up);
    down   = interface->inputPoll(port, id, Down);
    left   = interface->inputPoll(port, id, Left);
    right  = interface->inputPoll(port, id, Right);
  }
}
