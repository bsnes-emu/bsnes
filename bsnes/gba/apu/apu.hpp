struct APU : Thread {
  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
};

extern APU apu;
