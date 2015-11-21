struct Noise {
  auto dac_enable() const -> bool;

  auto run() -> void;
  auto clock_length() -> void;
  auto clock_envelope() -> void;
  auto write(uint r, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  uint4 envelope_volume;
  bool envelope_direction;
  uint3 envelope_frequency;
  uint4 frequency;
  bool narrow_lfsr;
  uint divisor;
  bool counter;

  int16 output;
  uint6 length;
  uint3 envelope_period;
  uint4 volume;
  uint period;
  uint15 lfsr;
};
