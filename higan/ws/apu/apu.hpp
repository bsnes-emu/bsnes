struct APU : Thread {
  static auto Enter() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;
};

extern APU apu;
