
struct Flash : SPIDevice, StaticMemory {
  ~Flash();
  Flash() {}
  Flash(uint32 esize, uint32 id);
  Flash(const stream& memory, uint32 esize, uint32 id);
  
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
  
  uint24   id;
  uint8    command;
  uint24   address, page;
  unsigned addrCount;
  unsigned count;
  uint8    buffer[256];
  bool     powered;
  bool     writeEnable;
};
