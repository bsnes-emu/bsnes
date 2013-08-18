namespace phoenix {

struct pKeyboard {
  static bool pressed(Keyboard::Scancode scancode);
  static vector<bool> state();
};

}
