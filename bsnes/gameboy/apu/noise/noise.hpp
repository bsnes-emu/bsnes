struct Noise {
  unsigned length;
  unsigned envelope_volume;
  bool envelope_direction;
  unsigned envelope_period;
  unsigned period;
  bool step;
  unsigned divisor;
  bool counter;

  void run();
  uint8 read(unsigned r);
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
