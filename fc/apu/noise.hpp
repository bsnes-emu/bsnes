struct Noise {
  auto clock_length() -> void;
  auto clock() -> uint8;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint length_counter;

  Envelope envelope;

  uint4 period;
  uint period_counter;

  bool short_mode;
  uint15 lfsr;
} noise;
