struct NECDSP : Processor::uPD96050, Coprocessor {
  unsigned Select;

  static void Enter();
  void enter();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  uint8 ram_read(unsigned addr);
  void ram_write(unsigned addr, uint8 data);

  void init();
  void load();
  void unload();
  void power();
  void reset();

  vector<uint8> firmware();
  void serialize(serializer&);
};

extern NECDSP necdsp;
