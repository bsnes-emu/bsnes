struct Wave {
  auto getPattern(uint5 offset) const -> uint4;

  auto run() -> void;
  auto clockLength() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power(bool initializeLength = true) -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  bool dacEnable;
  uint2 volume;
  uint11 frequency;
  bool counter;
  uint8 pattern[16];

  int16 output;
  uint length;
  uint period;
  uint5 patternOffset;
  uint4 patternSample;
  uint patternHold;
};
