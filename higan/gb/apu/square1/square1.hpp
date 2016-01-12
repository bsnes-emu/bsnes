struct Square1 {
  auto dacEnable() const -> bool;

  auto run() -> void;
  auto sweep(bool update) -> void;
  auto clockLength() -> void;
  auto clockSweep() -> void;
  auto clockEnvelope() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power(bool initializeLength = true) -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  uint3 sweepFrequency;
  bool sweepDirection;
  uint3 sweepShift;
  bool sweepNegate;
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
  uint3 sweepPeriod;
  int frequencyShadow;
  bool sweepEnable;
  uint4 volume;
};
