struct Envelope {
  auto volume() const -> uint;
  auto clock() -> void;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint4 speed;
  bool use_speed_as_volume;
  bool loop_mode;

  bool reload_decay;
  uint8 decay_counter;
  uint4 decay_volume;
};
