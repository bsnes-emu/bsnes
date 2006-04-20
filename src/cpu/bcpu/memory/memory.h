uint8 apu_port[4];
  inline uint8  port_read (uint8 port);
  inline void   port_write(uint8 port, uint8 value);

  inline void   cpu_io();
  inline uint8  mem_read(uint32 addr);
  inline void   mem_write(uint32 addr, uint8 value);
