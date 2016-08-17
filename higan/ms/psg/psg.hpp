//TI SN76489

struct PSG : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
};

extern PSG psg;
