struct Justifier : Controller {
  enum : uint {
    X, Y, Trigger, Start,
  };

  Justifier(bool port, bool chained);

  auto enter() -> void;
  auto data() -> uint2;
  auto latch(bool data) -> void;

//private:
  const bool chained;  //true if the second justifier is attached to the first
  const unsigned device;
  bool latched;
  unsigned counter;

  bool active;
  struct Player {
    signed x, y;
    bool trigger, start;
  } player1, player2;
};
