struct Square2 {
  unsigned duty;
  unsigned length;
  unsigned envelope_volume;
  unsigned envelope_direction;
  unsigned envelope_frequency;
  unsigned frequency;
  unsigned counter;

  int16 output;
  bool duty_output;
  unsigned phase;
  unsigned period;
  unsigned envelope_period;
  unsigned volume;

  void run();
  void clock_length();
  void clock_envelope();
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
