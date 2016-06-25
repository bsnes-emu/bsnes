Gamepad::Gamepad(bool port) : Controller(port) {
}

auto Gamepad::data() -> uint3 {
  if(counter >= 8) return 1;
  if(latched == 1) return interface->inputPoll(port, ID::Device::Gamepad, A);

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
    a      = interface->inputPoll(port, ID::Device::Gamepad, A);
    b      = interface->inputPoll(port, ID::Device::Gamepad, B);
    select = interface->inputPoll(port, ID::Device::Gamepad, Select);
    start  = interface->inputPoll(port, ID::Device::Gamepad, Start);
    up     = interface->inputPoll(port, ID::Device::Gamepad, Up);
    down   = interface->inputPoll(port, ID::Device::Gamepad, Down);
    left   = interface->inputPoll(port, ID::Device::Gamepad, Left);
    right  = interface->inputPoll(port, ID::Device::Gamepad, Right);
  }
}
