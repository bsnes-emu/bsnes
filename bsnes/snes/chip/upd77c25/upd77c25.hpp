class UPD77C25 : public Coprocessor {
public:
  static void Enter();
  void enter();
  void init();
  void enable();
  void power();
  void reset();

  void serialize(serializer &s);

  uint24 programROM[2048];
  uint16 dataROM[1024];
  uint16 dataRAM[256];

  string disassemble(uint11 ip);

private:
  struct Flag {
    bool s1, s0, c, z, ov1, ov0;

    inline operator unsigned() const {
      return (s1 << 5) + (s0 << 4) + (c << 3) + (z << 2) + (ov1 << 1) + (ov0 << 0);
    }

    inline unsigned operator=(unsigned d) {
      s1 = d & 0x20; s0 = d & 0x10; c = d & 0x08; z = d & 0x04; ov1 = d & 0x02; ov0 = d & 0x01;
      return d;
    }
  };

  struct Status {
    bool rqm, usf1, usf0, drs, dma, drc, soc, sic, ei, p1, p0;

    inline operator unsigned() const {
      return (rqm << 15) + (usf1 << 14) + (usf0 << 13) + (drs << 12)
           + (dma << 11) + (drc  << 10) + (soc  <<  9) + (sic <<  8)
           + (ei  <<  7) + (p1   <<  1) + (p0   <<  0);
    }

    inline unsigned operator=(unsigned d) {
      rqm = d & 0x8000; usf1 = d & 0x4000; usf0 = d & 0x2000; drs = d & 0x1000;
      dma = d & 0x0800; drc  = d & 0x0400; soc  = d & 0x0200; sic = d & 0x0100;
      ei  = d & 0x0080; p1   = d & 0x0002; p0   = d & 0x0001;
      return d;
    }
  };

  struct Regs {
    uint11 pc;        //program counter
    uint11 stack[4];  //LIFO
    uint10 rp;        //ROM pointer
    uint8  dp;        //data pointer
    int16  k;
    int16  l;
    int16  m;
    int16  n;
    int16  a;         //accumulator
    int16  b;         //accumulator
    Flag  flaga;
    Flag  flagb;
    uint16 tr;        //temporary register
    uint16 trb;       //temporary register
    Status sr;        //status register
    uint16 dr;        //data register
    bool siack;
    bool soack;
    uint16 idb;
  } regs;

  void exec_op(uint24 opcode);
  void exec_rt(uint24 opcode);
  void exec_jp(uint24 opcode);
  void exec_ld(uint24 opcode);

  void stack_push();
  void stack_pull();

  uint8 read(bool mode);
  void write(bool mode, uint8 data);

  friend class UPD77C25SR;
  friend class UPD77C25DR;
};

class UPD77C25SR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

class UPD77C25DR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern UPD77C25 upd77c25;
extern UPD77C25SR upd77c25sr;
extern UPD77C25DR upd77c25dr;
