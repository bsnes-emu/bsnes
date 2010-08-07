class DSP2 {
public:
  struct {
    bool waiting_for_command;
    unsigned command;
    unsigned in_count,  in_index;
    unsigned out_count, out_index;

    uint8  parameters[512];
    uint8  output[512];

    uint8  op05transparent;
    bool   op05haslen;
    int    op05len;
    bool   op06haslen;
    int    op06len;
    uint16 op09word1;
    uint16 op09word2;
    bool   op0dhaslen;
    int    op0doutlen;
    int    op0dinlen;
  } status;

  void init();
  void enable();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  void serialize(serializer&);
  DSP2();
  ~DSP2();

protected:
  void op01();
  void op03();
  void op05();
  void op06();
  void op09();
  void op0d();
};

class DSP2DR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

class DSP2SR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern DSP2 dsp2;
extern DSP2DR dsp2dr;
extern DSP2SR dsp2sr;
