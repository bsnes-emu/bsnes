/*****
 * CPU<>APU communication ports
 *****/
uint8 apu_port[4];
  uint8  port_read (uint8 port)             { return apu_port[port & 3]; }
  void   port_write(uint8 port, uint8 data) { apu_port[port & 3] = data; }

/*****
 * core CPU bus functions
 *****/
  void   op_io();
  uint8  op_read (uint32 addr);
  void   op_write(uint32 addr, uint8 data);

/*****
 * helper memory addressing functions used by CPU core
 *****/
  uint8  op_readpc    ();
  uint8  op_readstack ();
  uint8  op_readstackn();
  uint8  op_readaddr  (uint32 addr);
  uint8  op_readlong  (uint32 addr);
  uint8  op_readdbr   (uint32 addr);
  uint8  op_readpbr   (uint32 addr);
  uint8  op_readdp    (uint32 addr);
  uint8  op_readsp    (uint32 addr);

  void   op_writestack (uint8  data);
  void   op_writestackn(uint8  data);
  void   op_writeaddr  (uint32 addr, uint8 data);
  void   op_writelong  (uint32 addr, uint8 data);
  void   op_writedbr   (uint32 addr, uint8 data);
  void   op_writepbr   (uint32 addr, uint8 data);
  void   op_writedp    (uint32 addr, uint8 data);
  void   op_writesp    (uint32 addr, uint8 data);
