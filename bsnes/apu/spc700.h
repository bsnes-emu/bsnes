class sony_spc700 {
public:
  struct {
    word pc;
    byte a, x, y, sp, p;
    word dp;
  }regs;
  byte *ram;

  void Reset(void);
  void Run(void);

  void exec_op(void);
  word convert_address(byte mode, word addr);
  byte mem_getbyte(word addr);
  void mem_putbyte(word addr, byte value);
  word mem_read(byte mode, byte size, word addr);
  void mem_write(byte mode, byte size, word addr, word value);
  byte stack_read(void);
  void stack_write(byte value);

  sony_spc700();
  ~sony_spc700();
};
