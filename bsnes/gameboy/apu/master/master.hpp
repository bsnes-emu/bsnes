struct Master {
  bool so2_enable;
  unsigned so2_volume;
  bool so1_enable;
  unsigned so1_volume;
  bool channel4_so2_enable;
  bool channel3_so2_enable;
  bool channel2_so2_enable;
  bool channel1_so2_enable;
  bool channel4_so1_enable;
  bool channel3_so1_enable;
  bool channel2_so1_enable;
  bool channel1_so1_enable;
  bool enable;
  bool channel4_enable;
  bool channel3_enable;
  bool channel2_enable;
  bool channel1_enable;

  void run();
  uint8 read(unsigned r);
  void write(unsigned r, uint8 data);
  void power();
  void serialize(serializer&);
};
