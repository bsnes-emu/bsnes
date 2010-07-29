class Serial : public Processor, public library, public property<Serial> {
public:
  //synchronization
  alwaysinline void create();
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

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

  function<void (void (*)(unsigned), uint8_t (*)(), void (*)(uint8_t))> snesserial_main;
};

extern Serial serial;
