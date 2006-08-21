  uint8  spcram_read (uint16 addr);
  void   spcram_write(uint16 addr, uint8 data);

  uint8  port_read (uint8 port);
  void   port_write(uint8 port, uint8 data);

/*****
 * core APU bus functions
 *****/
  inline void   op_io   ();
  inline uint8  op_read (uint16 addr);
  inline void   op_write(uint16 addr, uint8 data);

/*****
 * helper memory addressing functions used by APU core
 *****/
  inline uint8  op_readpc    ();
  inline uint8  op_readstack ();
  inline void   op_writestack(uint8  data);
  inline uint8  op_readaddr  (uint16 addr);

  inline void   op_writeaddr (uint16 addr, uint8 data);
  inline uint8  op_readdp    (uint8  addr);
  inline void   op_writedp   (uint8  addr, uint8 data);
