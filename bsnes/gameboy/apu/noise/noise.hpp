struct Noise {
  bool enable;

  unsigned envelope_volume;
  bool envelope_direction;
  unsigned envelope_frequency;
  unsigned frequency;
  bool narrow_lfsr;
  unsigned divisor;
  bool counter;

  int16 output;
  unsigned length;
  unsigned envelope_period;
  unsigned volume;
  unsigned period;
  uint15 lfsr;

  bool dac_enable();

  void run();
  void clock_length();
  void clock_envelope();
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
