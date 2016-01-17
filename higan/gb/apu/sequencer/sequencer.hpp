struct Sequencer {
  auto run() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool leftEnable;
  uint3 leftVolume;
  bool rightEnable;
  uint3 rightVolume;

  struct Channel {
    bool leftEnable;
    bool rightEnable;
  } square1, square2, wave, noise;

  bool enable;

  int16 center;
  int16 left;
  int16 right;

  int64 centerBias;
  int64 leftBias;
  int64 rightBias;
};
