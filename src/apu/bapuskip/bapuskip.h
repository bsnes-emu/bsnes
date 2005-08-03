class bAPUSkip : public APU {
public:

struct _apu_port {
  uint8  value;
  uint8  step, pos;
  uint32 read_addr[32], read_pos;
}apu_port[4];

enum {
  OP_CMP_CONST = 0xc9,
  OP_CPX_CONST = 0xe0,
  OP_CPY_CONST = 0xc0,
  OP_CMP_ADDR  = 0xcd,
  OP_CPX_ADDR  = 0xec,
  OP_CPY_ADDR  = 0xcc,
  OP_CMP_LONG  = 0xcf,
  OP_BNE       = 0xd0,
  OP_BEQ       = 0xf0,
  OP_BRA       = 0x80,
  OP_NOP       = 0xea
};

  uint8  spcram_read (uint16 addr);
  void   spcram_write(uint16 addr, uint8 value);
  uint8  port_read   (uint8 port);
  void   port_write  (uint8 port, uint8 value);

  void   run();
  uint32 cycles_executed() { return 12 * 24; }
  void   power();
  void   reset();
};
