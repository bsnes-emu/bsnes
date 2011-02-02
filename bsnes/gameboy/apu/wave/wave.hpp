struct Wave {
  bool off;
  unsigned length;
  unsigned volume;
  unsigned frequency;
  bool counter;
  uint8 pattern[32];

  void run();
  uint8 read(unsigned r);
  void write(unsigned r, uint8 data);
  uint8 read_pattern(unsigned p);
  void write_pattern(unsigned p, uint8 data);
  void power();
  void serialize(serializer&);
};
