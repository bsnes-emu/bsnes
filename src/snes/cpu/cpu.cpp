#include <snes/snes.hpp>

#define CPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "cpu-debugger.cpp"
#endif

void CPU::Enter() { cpu.enter(); }

void CPU::power() {
  create(CPU::Enter, system.cpu_frequency());
  coprocessors.reset();

  cpu_version = config.cpu.version;
}

void CPU::reset() {
  create(CPU::Enter, system.cpu_frequency());
  coprocessors.reset();

  PPUCounter::reset();
}

void CPU::serialize(serializer &s) {
  Processor::serialize(s);
  PPUCounter::serialize(s);
  s.integer(cpu_version);
}

CPU::CPU() {
}

CPU::~CPU() {
}

}
