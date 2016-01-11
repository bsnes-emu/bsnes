struct Square2 {
  auto dac_enable() const -> bool;

  auto run() -> void;
  auto clock_length() -> void;
  auto clock_envelope() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool enable;

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
  uint4 volume;
};
