#include "core/core.hpp"

struct CPU : Processor, ARM {
  StaticMemory iram;
  StaticMemory eram;

  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();

  CPU();
};

extern CPU cpu;
