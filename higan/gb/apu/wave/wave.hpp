struct Wave {
  auto get_pattern(uint5 offset) const -> uint4;

  auto run() -> void;
  auto clock_length() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  bool dac_enable;
  uint2 volume;
  uint11 frequency;
  bool counter;
  uint8 pattern[16];

  int16 output;
  uint8 length;
  uint period;
  uint5 pattern_offset;
  uint4 pattern_sample;
};
