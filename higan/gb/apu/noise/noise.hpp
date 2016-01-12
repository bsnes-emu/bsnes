struct Noise {
  auto dacEnable() const -> bool;
  auto getPeriod() const -> uint;

  auto run() -> void;
  auto clockLength() -> void;
  auto clockEnvelope() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power(bool initializeLength = true) -> void;

  auto serialize(serializer&) -> void;

  bool enable;

  uint4 envelopeVolume;
  bool envelopeDirection;
  uint3 envelopeFrequency;
  uint4 frequency;
  bool narrow;
  uint3 divisor;
  bool counter;

  int16 output;
  uint length;
  uint3 envelopePeriod;
  uint4 volume;
  uint period;
  uint15 lfsr;
};
