class Serial : property<Serial> {
public:
  void enter();

  void init();
  void enable();
  void power();
  void reset();

  readonly<bool> latch;

private:
  void add_clocks(unsigned clocks);
};

extern Serial serial;
