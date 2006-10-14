  uint8  ram_read (uint16 addr);
  void   ram_write(uint16 addr, uint8 data);

  uint8  port_read (uint8 port);
  void   port_write(uint8 port, uint8 data);

/*****
 * core SMP bus functions
 *****/
  uint8  op_busread (uint16 addr);
  void   op_buswrite(uint16 addr, uint8 data);

  void   op_io   ();
  uint8  op_read (uint16 addr);
  void   op_write(uint16 addr, uint8 data);

/*****
 * helper memory addressing functions used by SMP core
 *****/
  uint8  op_readpc    ()                        { return op_read(regs.pc++); }

  uint8  op_readstack ()                        { return op_read(0x0100 | ++regs.sp); }
  void   op_writestack(uint8  data)             { op_write(0x0100 | regs.sp--, data); }

  uint8  op_readaddr  (uint16 addr)             { return op_read(addr); }
  void   op_writeaddr (uint16 addr, uint8 data) { op_write(addr, data); }

  uint8  op_readdp    (uint8  addr)             { return op_read(((uint)regs.p.p << 8) + addr); }
  void   op_writedp   (uint8  addr, uint8 data) { op_write(((uint)regs.p.p << 8) + addr, data); }
