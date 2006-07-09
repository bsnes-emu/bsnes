enum { OPMODE_ADDR, OPMODE_DP };
  inline uint8  spcram_read (uint16 addr);
  inline void   spcram_write(uint16 addr, uint8 value);
  inline uint8  port_read (uint8 port);
  inline void   port_write(uint8 port,  uint8 value);

  inline uint8  op_read();
  inline uint8  op_read (uint8 mode, uint16 addr);
  inline void   op_write(uint8 mode, uint16 addr, uint8 value);
  inline uint8  stack_read();
  inline void   stack_write(uint8 value);
