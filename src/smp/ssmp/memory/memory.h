  uint8 ram_read(uint16 addr);
  void ram_write(uint16 addr, uint8 data);

  uint8 port_read(uint8 port);
  void port_write(uint8 port, uint8 data);

/*****
 * core SMP bus functions
 *****/
  uint8 op_busread(uint16 addr);
  void op_buswrite(uint16 addr, uint8 data);

  void op_io();
  uint8 op_read(uint16 addr);
  void op_write(uint16 addr, uint8 data);

/*****
 * helper memory addressing functions used by SMP core
 *****/
  uint8 op_readpc();

  uint8 op_readstack();
  void op_writestack(uint8 data);

  uint8 op_readaddr(uint16 addr);
  void op_writeaddr(uint16 addr, uint8 data);

  uint8 op_readdp(uint8 addr);
  void op_writedp(uint8 addr, uint8 data);
