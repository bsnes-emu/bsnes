bool pKeyboard::pressed(Keyboard::Scancode scancode) {
  return false;
}

vector<bool> pKeyboard::state() {
  vector<bool> output;
  output.resize((unsigned)Keyboard::Scancode::Limit);
  for(auto &n : output) n = false;
  return output;
}
