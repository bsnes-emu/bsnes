struct Pulse {
  auto clock_length() -> void;
  auto check_period() -> bool;
  auto clock() -> uint8;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint length_counter;

  Envelope envelope;
  Sweep sweep;

  uint2 duty;
  uint3 duty_counter;

  uint11 period;
  uint period_counter;
} pulse[2];
