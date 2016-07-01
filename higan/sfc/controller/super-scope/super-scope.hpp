struct SuperScope : Controller {
  shared_pointer<Emulator::Sprite> sprite;

  enum : uint {
    X, Y, Trigger, Cursor, Turbo, Pause,
  };

  SuperScope(bool port);
  ~SuperScope();

  auto main() -> void;
  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;

  int x;
  int y;

  bool trigger;
  bool cursor;
  bool turbo;
  bool pause;
  bool offscreen;

  bool oldturbo;
  bool triggerlock;
  bool pauselock;

  uint prev;
};
