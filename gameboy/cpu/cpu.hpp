struct CPU : Processor {
  #include "core/core.hpp"
  #include "timing/timing.hpp"

  struct Status {
    unsigned lycounter;
  } status;

  static void Main();
  void main();
  void power();
  void reset();
  CPU();
};

extern  CPU cpu;
