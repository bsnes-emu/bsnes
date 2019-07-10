struct Justifier : Controller {
  enum : uint {
    X, Y, Trigger, Start,
  };

  Justifier(uint port, bool chained);
  ~Justifier();

  auto data() -> uint2;
  auto latch(bool data) -> void;

//private:
  const bool chained;  //true if the second justifier is attached to the first
  const uint device;
  bool latched;
  uint counter;
  uint prev;

  bool active;
  struct Player {
    shared_pointer<Emulator::Sprite> sprite;
    int x;
    int y;
    bool trigger;
    bool start;
  } player1, player2;
};
