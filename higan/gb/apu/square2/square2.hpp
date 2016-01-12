struct Square2 {
  auto dacEnable() const -> bool;

  auto run() -> void;
  auto clockLength() -> void;
  auto clockEnvelope() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power(bool initializeLength = true) -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  uint2 duty;
  uint length;
  uint4 envelopeVolume;
  bool envelopeDirection;
  uint3 envelopeFrequency;
  uint11 frequency;
  bool counter;

  int16 output;
  bool dutyOutput;
  uint3 phase;
  uint period;
  uint3 envelopePeriod;
  uint4 volume;
};
