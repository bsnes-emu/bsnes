struct SuperScope : Controller {
  SuperScope(bool port);

  auto enter() -> void;
  auto data() -> uint2;
  auto latch(bool data) -> void;

//private:
  bool latched;
  unsigned counter;

  signed x, y;

  bool trigger;
  bool cursor;
  bool turbo;
  bool pause;
  bool offscreen;

  bool turbolock;
  bool triggerlock;
  bool pauselock;
};
