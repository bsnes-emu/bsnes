struct Master {
  auto run() -> void;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  bool left_in_enable;
  uint3 left_volume;
  bool right_in_enable;
  uint3 right_volume;
  bool channel4_left_enable;
  bool channel3_left_enable;
  bool channel2_left_enable;
  bool channel1_left_enable;
  bool channel4_right_enable;
  bool channel3_right_enable;
  bool channel2_right_enable;
  bool channel1_right_enable;
  bool enable;

  int16 center;
  int16 left;
  int16 right;

  int64 center_bias;
  int64 left_bias;
  int64 right_bias;
};
