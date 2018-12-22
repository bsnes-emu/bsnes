struct NumberPad : Controller {
  enum : uint {
    Up, Down, Left, Right,
    L, R,
    One, Two, Three,
    Four, Five, Six,
    Seven, Eight, Nine,
    Star, Zero, Pound,
  };

  NumberPad(uint port);

  auto readData() -> uint8;
  auto writeData(uint8 data) -> void;

  uint1 select;
};
