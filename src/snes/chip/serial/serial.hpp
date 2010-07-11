class Serial : property<Serial>, public library {
public:
  void enter();

  void init();
  void enable();
  void power();
  void reset();

  readonly<bool> latch;

  void add_clocks(unsigned clocks);
  uint8 read();
  void write(uint8 data);

  function<void (void (*)(unsigned), uint8_t (*)(), void (*)(uint8_t))> snesserial_main;
};

extern Serial serial;
