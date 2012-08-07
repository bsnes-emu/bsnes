
struct EEPROM : SPIDevice, StaticMemory {
  ~EEPROM();
  EEPROM(uint32 esize, uint32 psize);
  EEPROM(const stream& memory, uint32 esize, uint32 psize);
  
  void power();
  void select(bool state);
  uint8 transfer(uint8 data);
  
  uint8    command;
  uint16   address, page, pageSize;
  unsigned addrCount;
  unsigned count;
  uint8    buffer[256];
  bool     writeEnable;
};
