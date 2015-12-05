struct Triangle {
  auto clock_length() -> void;
  auto clock_linear_length() -> void;
  auto clock() -> uint8;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint length_counter;

  uint8 linear_length;
  bool halt_length_counter;

  uint11 period;
  uint period_counter;

  uint5 step_counter;
  uint8 linear_length_counter;
  bool reload_linear;
} triangle;
