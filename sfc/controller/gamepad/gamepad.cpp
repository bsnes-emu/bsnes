#ifdef CONTROLLER_CPP

Gamepad::Gamepad(bool port) : Controller(port) {
  latched = 0;
  counter = 0;

  b = y = select = start = 0;
  up = down = left = right = 0;
  a = x = l = r = 0;
}

auto Gamepad::data() -> uint2 {
  if(counter >= 16) return 1;
  if(latched == 1) return interface->inputPoll(port, (unsigned)Input::Device::Joypad, (unsigned)Input::JoypadID::B);

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
    unsigned id = (unsigned)Input::Device::Joypad;
    b      = interface->inputPoll(port, id,  0);
    y      = interface->inputPoll(port, id,  1);
    select = interface->inputPoll(port, id,  2);
    start  = interface->inputPoll(port, id,  3);
    up     = interface->inputPoll(port, id,  4);
    down   = interface->inputPoll(port, id,  5);
    left   = interface->inputPoll(port, id,  6);
    right  = interface->inputPoll(port, id,  7);
    a      = interface->inputPoll(port, id,  8);
    x      = interface->inputPoll(port, id,  9);
    l      = interface->inputPoll(port, id, 10);
    r      = interface->inputPoll(port, id, 11);
  }
}

#endif
