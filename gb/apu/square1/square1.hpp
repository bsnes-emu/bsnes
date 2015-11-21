struct Square1 {
  auto dac_enable() const -> bool;

  auto run() -> void;
  auto sweep(bool update) -> void;
  auto clock_length() -> void;
  auto clock_sweep() -> void;
  auto clock_envelope() -> void;
  auto write(uint r, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  uint3 sweep_frequency;
  bool sweep_direction;
  uint3 sweep_shift;
  bool sweep_negate;
  uint2 duty;
  uint6 length;
  uint4 envelope_volume;
  bool envelope_direction;
  uint3 envelope_frequency;
  uint11 frequency;
  bool counter;

  int16 output;
  bool duty_output;
  uint3 phase;
  uint period;
  uint3 envelope_period;
  uint3 sweep_period;
  int frequency_shadow;
  bool sweep_enable;
  uint4 volume;
};
