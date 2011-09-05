struct Square1 {
  bool enable;

  unsigned sweep_frequency;
  unsigned sweep_direction;
  unsigned sweep_shift;
  bool sweep_negate;
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
  unsigned sweep_period;
  signed frequency_shadow;
  bool sweep_enable;
  unsigned volume;

  bool dac_enable();

  void run();
  void sweep(bool update);
  void clock_length();
  void clock_sweep();
  void clock_envelope();
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
