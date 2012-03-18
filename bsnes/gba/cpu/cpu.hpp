#include "core/core.hpp"

struct CPU : Processor, ARM7TDMI {
  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
};

extern CPU cpu;
