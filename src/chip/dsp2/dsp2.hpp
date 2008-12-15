class DSP2 : public Memory {
public:
  struct {
    bool   waiting_for_command;
    uint   command;
    uint   in_count,  in_index;
    uint   out_count, out_index;

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

  uint8 read(uint addr);
  void write(uint addr, uint8 data);

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

extern DSP2 dsp2;
