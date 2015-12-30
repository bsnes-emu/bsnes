Gamepad::Gamepad(bool port) : Controller(port) {
  latched = 0;
  counter = 0;

  b = y = select = start = 0;
  up = down = left = right = 0;
  a = x = l = r = 0;
}

auto Gamepad::data() -> uint2 {
  if(counter >= 16) return 1;
  if(latched == 1) return interface->inputPoll(port, (unsigned)Device::ID::Gamepad, B);

  //note: D-pad physically prevents up+down and left+right from being pressed at the same time
  switch(counter++) {
  case  0: return b;
  case  1: return y;
  case  2: return select;
  case  3: return start;
  case  4: return up & !down;
  case  5: return down & !up;
  case  6: return left & !right;
  case  7: return right & !left;
  case  8: return a;
  case  9: return x;
  case 10: return l;
  case 11: return r;
  }

  return 0;  //12-15: signature
}

auto Gamepad::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    auto id = (unsigned)Device::ID::Gamepad;
    b      = interface->inputPoll(port, id, B);
    y      = interface->inputPoll(port, id, Y);
    select = interface->inputPoll(port, id, Select);
    start  = interface->inputPoll(port, id, Start);
    up     = interface->inputPoll(port, id, Up);
    down   = interface->inputPoll(port, id, Down);
    left   = interface->inputPoll(port, id, Left);
    right  = interface->inputPoll(port, id, Right);
    a      = interface->inputPoll(port, id, A);
    x      = interface->inputPoll(port, id, X);
    l      = interface->inputPoll(port, id, L);
    r      = interface->inputPoll(port, id, R);
  }
}
