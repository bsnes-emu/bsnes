struct SuperDisc : Coprocessor, Memory {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

private:
  uint8 r21e0;
  uint8 r21e1;
  uint8 r21e2;
  uint8 r21e3;
  uint8 r21e4;
  uint8 r21e5;
};

extern SuperDisc superdisc;
