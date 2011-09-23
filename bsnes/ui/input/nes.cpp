int16_t NesGamepad::poll(unsigned n) {
  switch(n) {
  case 0: return a.poll();
  case 1: return b.poll();
  case 2: return select.poll();
  case 3: return start.poll();
  case 4: return up.poll() & !down.poll();
  case 5: return down.poll() & !up.poll();
  case 6: return left.poll() & !right.poll();
  case 7: return right.poll() & !left.poll();
  }
  return 0;
}

NesGamepad::NesGamepad() {
  name = "Gamepad";

  up.name = "Up";
  down.name = "Down";
  left.name = "Left";
  right.name = "Right";
  b.name = "B";
  a.name = "A";
  select.name = "Select";
  start.name = "Start";

  up.mapping = "KB0::Up";
  down.mapping = "KB0::Down";
  left.mapping = "KB0::Left";
  right.mapping = "KB0::Right";
  b.mapping = "KB0::Z";
  a.mapping = "KB0::X";
  select.mapping = "KB0::Apostrophe";
  start.mapping = "KB0::Return";

  append(up); append(down); append(left); append(right);
  append(b); append(a); append(select); append(start);
}

//

NesPort1Input::NesPort1Input() {
  name = "Controller Port 1";
  append(gamepad);
}

//

NesPort2Input::NesPort2Input() {
  name = "Controller Port 2";
  append(gamepad);
}

//

NesInput::NesInput() {
  name = "NES";
  append(port1);
  append(port2);
}
