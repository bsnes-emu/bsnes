class NECDSP : public Coprocessor, public Memory {
public:
  enum class Revision : unsigned { uPD7725, uPD96050 } revision;
  unsigned frequency;
  unsigned drmask, drtest;
  unsigned srmask, srtest;
  unsigned dpmask, dptest;

  #include "registers.hpp"

  uint24 programROM[16384];
  uint16 dataROM[2048];
  uint16 dataRAM[2048];

  unsigned programROMSize;
  unsigned dataROMSize;
  unsigned dataRAMSize;

  static void Enter();
  void enter();

  void exec();
  void exec_op(uint24 opcode);
  void exec_rt(uint24 opcode);
  void exec_jp(uint24 opcode);
  void exec_ld(uint24 opcode);

  string disassemble(uint14 ip);

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  uint8 sr_read();
  void sr_write(uint8 data);

  uint8 dr_read();
  void dr_write(uint8 data);

  uint8 dp_read(unsigned addr);
  void dp_write(unsigned addr, uint8 data);

  void init();
  void enable();
  void power();
  void reset();

  void serialize(serializer&);
  NECDSP();
  ~NECDSP();
};

extern NECDSP necdsp;
