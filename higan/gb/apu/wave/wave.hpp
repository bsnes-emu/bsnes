struct Wave {
  auto run() -> void;
  auto clock_length() -> void;
  auto write(uint r, uint8 data) -> void;
  auto write_pattern(uint p, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  bool dac_enable;
  uint volume_shift;
  uint11 frequency;
  bool counter;
  uint8 pattern[32];

  int16 output;
  uint8 length;
  uint period;
  uint5 pattern_offset;
  uint4 pattern_sample;
};
