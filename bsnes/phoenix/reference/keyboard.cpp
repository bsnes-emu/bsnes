bool pKeyboard::pressed(Keyboard::Scancode scancode) {
  return false;
}

array<bool> pKeyboard::state() {
  array<bool> output;
  output.resize((unsigned)Keyboard::Scancode::Limit);
  for(auto &n : output) n = false;
  return output;
}
