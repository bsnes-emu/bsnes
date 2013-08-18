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
  bool txbusy;
  bool rxbusy;
  bool txlatch;
  vector<uint8> txbuffer;
  vector<uint8> rxbuffer;
};

extern HSU1 hsu1;
