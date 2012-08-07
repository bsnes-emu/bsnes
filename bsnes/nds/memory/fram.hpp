
struct FRAM : SPIDevice, StaticMemory {
  ~FRAM();
  FRAM(uint32 esize);
  FRAM(const stream& memory, uint32 esize);
  
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
  
  uint8    command;
  uint16   address, page, pageSize;
  unsigned addrCount;
  uint8    buffer[256];
  bool     writeEnable;
};
