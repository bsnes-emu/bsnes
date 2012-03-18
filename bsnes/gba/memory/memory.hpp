struct Bus {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern Bus bus;
