struct SuperScope : Controller {
  uint2 data();
  void latch(bool data);
  SuperScope(bool port);

//private:
  bool latched;
  unsigned counter;

  int x, y;

  bool trigger;
  bool cursor;
  bool turbo;
  bool pause;
  bool offscreen;

  bool turbolock;
  bool triggerlock;
  bool pauselock;
};
