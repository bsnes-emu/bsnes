//TI SN76489

struct PSG : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;
  auto reset() -> void;
};

extern PSG psg;
