struct Gamepad : Controller {
  uint2 data();
  void latch(bool data);
  Gamepad(bool port);

private:
  bool latched;
  unsigned counter;

  bool b, y, select, start;
  bool up, down, left, right;
  bool a, x, l, r;
};
