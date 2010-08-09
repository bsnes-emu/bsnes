class Serial : public Coprocessor, public MMIO, public library, public property<Serial> {
public:
  static void Enter();
  void enter();
  void init();
  void enable();
  void power();
  void reset();
  void serialize(serializer&);

  readonly<bool> latch;

  void add_clocks(unsigned clocks);
  uint8 read();
  void write(uint8 data);

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

private:
  MMIO *r4016, *r4017;
  function<void (void (*)(unsigned), uint8_t (*)(), void (*)(uint8_t))> snesserial_main;
};

extern Serial serial;
