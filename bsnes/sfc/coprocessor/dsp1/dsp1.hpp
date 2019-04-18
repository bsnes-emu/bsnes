struct DSP1 {
  auto power() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;
};

extern DSP1 dsp1;
