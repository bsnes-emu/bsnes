struct HSU1 {
  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  void serialize(serializer&);

private:
  uint8 packet[512];
  unsigned packetlength;
};

extern HSU1 hsu1;
