#include <snes/snes.hpp>

#define CPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "cpu-debugger.cpp"
#endif

void CPU::power() {
  create();
  cpu_version = config.cpu.version;
}

void CPU::reset() {
  create();
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
