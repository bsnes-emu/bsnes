struct Square {
  bool has_sweep;

  unsigned sweep_period;
  unsigned sweep_direction;
  unsigned sweep_shift;
  unsigned duty;
  unsigned length;
  unsigned envelope_volume;
  unsigned envelope_direction;
  unsigned envelope_period;
  unsigned frequency;
  unsigned counter;

  void run();
  uint8 read(unsigned r);
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
