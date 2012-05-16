#ifdef CONTROLLER_CPP

uint2 Mouse::data() {
  if(counter >= 32) return 1;

  switch(counter++) { default:
  case  0: return 0;
  case  1: return 0;
  case  2: return 0;
  case  3: return 0;
  case  4: return 0;
  case  5: return 0;
  case  6: return 0;
  case  7: return 0;

  case  8: return input.r;
  case  9: return input.l;
  case 10: return 0;  //speed (0 = slow, 1 = normal, 2 = fast, 3 = unused)
  case 11: return 0;  // ||

  case 12: return 0;  //signature
  case 13: return 0;  // ||
  case 14: return 0;  // ||
  case 15: return 1;  // ||

  case 16: return input.dy;
  case 17: return (input.y >> 6) & 1;
  case 18: return (input.y >> 5) & 1;
  case 19: return (input.y >> 4) & 1;
  case 20: return (input.y >> 3) & 1;
  case 21: return (input.y >> 2) & 1;
  case 22: return (input.y >> 1) & 1;
  case 23: return (input.y >> 0) & 1;

  case 24: return input.dx;
  case 25: return (input.x >> 6) & 1;
  case 26: return (input.x >> 5) & 1;
  case 27: return (input.x >> 4) & 1;
  case 28: return (input.x >> 3) & 1;
  case 29: return (input.x >> 2) & 1;
  case 30: return (input.x >> 1) & 1;
  case 31: return (input.x >> 0) & 1;
  }
}

void Mouse::latch(bool data) {
  if(latched == data) return;
  latched = data;
  counter = 0;

  input.x = interface->inputPoll(port, (unsigned)Input::Device::Mouse, (unsigned)Input::MouseID::X);  //-n = left, 0 = center, +n = right
  input.y = interface->inputPoll(port, (unsigned)Input::Device::Mouse, (unsigned)Input::MouseID::Y);  //-n = up,   0 = center, +n = down
  input.l = interface->inputPoll(port, (unsigned)Input::Device::Mouse, (unsigned)Input::MouseID::Left );
  input.r = interface->inputPoll(port, (unsigned)Input::Device::Mouse, (unsigned)Input::MouseID::Right);

  input.dx = input.x < 0;  //0 = right, 1 = left
  input.dy = input.y < 0;  //0 = down,  1 = up

  if(input.x < 0) input.x = -input.x;  //abs(position_x)
  if(input.y < 0) input.y = -input.y;  //abs(position_y)

  input.x = min(127, input.x);
  input.y = min(127, input.y);
}

Mouse::Mouse(bool port) : Controller(port) {
  latched = 0;
  counter = 0;

  input.x = 0;
  input.y = 0;
  input.dx = 0;
  input.dy = 0;
  input.l = 0;
  input.r = 0;
}

#endif
